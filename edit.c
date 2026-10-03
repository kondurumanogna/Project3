#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"type.h"
#include"edit.h"

Status open_files_edit(Mp3tag *mp3Tag)
{
    mp3Tag->fptr_mp3=fopen(mp3Tag->mp3_filename,"rb");

    if(mp3Tag->fptr_mp3==NULL)
    {
        printf("ERROR : Unable to open the file\n");
        return e_failure;
    }

    mp3Tag->temp_filename="temp.mp3";

    mp3Tag->fptr_temp_mp3=fopen(mp3Tag->temp_filename,"wb");

    if(mp3Tag->fptr_temp_mp3==NULL)
    {
        printf("ERROR : Unable to create the file\n");
        return e_failure;
    }

    return e_success;
}
Status check_signature_edit(Mp3tag *mp3Tag)
{
    //moving offset to start of file
    fseek(mp3Tag->fptr_mp3,0,SEEK_SET);
    char *buffer = malloc(3*sizeof(char));
    if(buffer==NULL)
    {
        return e_failure;
    }
    //reading 3 bytes of header
    if(fread(buffer,1,3,mp3Tag->fptr_mp3)!=3)
    {
        printf("Error while reading the file\n");
        free(buffer);
        return e_failure;
    }
    
    //validating headr
    if(buffer[0]!='I')
    {
        printf("Error : Invalid Signature\n");
        free(buffer);
        return e_failure;
    }
    if(buffer[1]!='D')
    {
        printf("Error : Invalid Signature\n");
        free(buffer);
        return e_failure;
    }
    if(buffer[2]!='3')
    {
        printf("Error : Invalid Signature\n");
        free(buffer);
        return e_failure;

    }

    free(buffer);
    //moving offset to tag
    fseek(mp3Tag->fptr_mp3,10,SEEK_SET);
    return e_success;

}

Status validate_args_edit(char *argv[],Mp3tag *mp3Tag)
{
    int len=strlen(argv[4]);
    if(len<4)
    {
        printf("Entered INVALID INPUT\n");
        return e_failure;
    }

    if(argv[4][len-4]!='.')
    {
        printf("Enter Extension of the file\n");
        printf("To Edit please pass : ./a.out -e -c/-a/-A/-m/-y/-c mp3filename\n");
        return e_failure;
    }
    if(argv[4][len-3]!='m')
    {
        printf("Enter Extension of the file correctly\nThere should be one 'm' in the extension\n");
        printf("To Edit please pass : ./a.out -e -c/-a/-A/-m/-y/-c mp3filename\n");
        return e_failure;
    }
    if(argv[4][len-2]!='p') 
    {
        printf("Enter Extension of the file correctly\nThere should be one 'p' in the extension\n");
        printf("To Edit please pass : ./a.out -e -c/-a/-A/-m/-y/-c mp3filename\n");
        return e_failure;
    }
    if(argv[4][len-1]!='3')
    {
        printf("Enter Extension of the file\n");
        printf("To Edit please pass : ./a.out -e -c/-a/-A/-m/-y/-c mp3filename\n");
        return e_failure;
    }
    
    mp3Tag->mp3_filename=argv[4];

    if(open_files_edit(mp3Tag)==e_failure)
    {
        printf("ERROR : Unable to open the file\n");
        return e_failure;
    }

    //check mp3 signature
    if(check_signature_edit(mp3Tag)==e_failure)
    {
        printf("ERROR : Invalid Signature\n");
        return e_failure;
    }
    return e_success;
}

Status do_editing(char *argv[],Mp3tag *mp3Tag)
{
    if(copy_mp3_header(mp3Tag->fptr_mp3,mp3Tag->fptr_temp_mp3)==e_failure)
    {
        printf("Error While Copying!!\n");
        return e_failure;
    }
    //move offset to 10th byte
    fseek(mp3Tag->fptr_mp3,10,SEEK_SET);

    char option=argv[2][1];
    char *target=NULL;

    switch(option)
    {
        case 't':
            target="TPE1";
            break;
        case 'a':
            target="TIT2";
            break;
        case 'A':
            target="TALB";
            break;
        case 'y':
            target="TYER";
            break;
        case 'm':
            target="TCON";
            break;
        case 'c':
            target="COMM";
            break;
    }

    
    
    while(1)
    {
        char *tag=malloc(5*sizeof(char));
        if(tag==NULL)
        {
            printf("Error While reading tag\n");
            return e_failure;
        }
        
        if(fread(tag,1,4,mp3Tag->fptr_mp3)!=4)
        {
            free(tag);
            printf("Tag not found\n");
            return e_failure;
        }
        tag[4]='\0';
    

        unsigned char *size=malloc(4*sizeof(char));

        if(size==NULL)
        {
            free(tag);
            return e_failure;
        }

        if(fread(size,1,4,mp3Tag->fptr_mp3)!=4)
        {
            printf("Error While reading\n");
            free(tag);
            free(size);
            return e_failure;
        }

        //convert big endian  to little
        int start=0,end=3;
        while(start<=end)
        {
            char temp=size[start];
            size[start]=size[end];
            size[end]=temp;
            start++;
            end--;

        }
        //char into integer value
        union 
        {
           char byte[4];
           int value;
           
        }Size;
        
        for(int i=0;i<4;i++)
        {
            Size.byte[i]=size[i];
        }

        //If target is not found
        if(strcmp(tag,target)!=0)
        {
            //copy tag
            fwrite(tag,1,4,mp3Tag->fptr_temp_mp3);

           
            int start=0,end=3;
            while(start<=end)
            {
                char temp=size[start];
                size[start]=size[end];
                size[end]=temp;

                start++;
                end--;
            } 
            
            //copy size
            fwrite(size,1,4,mp3Tag->fptr_temp_mp3);
            

            //copy 3 bytes
            char *buffer=malloc(3*sizeof(char));

            if(buffer==NULL)
            {
                printf("Error while allocating memory to flags\n");
                free(tag);
                free(size);
                return e_failure;
            }
            fread(buffer,1,3,mp3Tag->fptr_mp3);
            fwrite(buffer,1,3,mp3Tag->fptr_temp_mp3);

            //copy metadata
            char *data=malloc(Size.value);
            if(data==NULL)
            {
                printf("Error while allocating memory!!\n");
                free(tag);
                free(size);
                free(buffer);
                return e_failure;
            }
            fread(data,1,Size.value-1,mp3Tag->fptr_mp3);
            fwrite(data,1,Size.value-1,mp3Tag->fptr_temp_mp3);

            
            free(buffer);
            free(data);


        }
        //if target found
        else
        {
            int len=strlen(argv[3]);

            int new_size=len+1;

            union
            {
                char byte[4];
                int value;
            }NewSize;

            NewSize.value=new_size;

            int start=0,end=3;
            while(start<=end)
            {
                char temp=NewSize.byte[start];
                NewSize.byte[start]=NewSize.byte[end];
                NewSize.byte[end]=temp;

                start++;
                end--;
            }

            fwrite(tag,1,4,mp3Tag->fptr_temp_mp3);
            
            fwrite(NewSize.byte,1,4,mp3Tag->fptr_temp_mp3);


            char *buffer1=malloc(3*sizeof(char));
            fread(buffer1,1,3,mp3Tag->fptr_mp3);
            fwrite(buffer1,1,3,mp3Tag->fptr_temp_mp3);

            fwrite(argv[3],1,len,mp3Tag->fptr_temp_mp3);
            free(buffer1);

            fseek(mp3Tag->fptr_mp3,Size.value-1,SEEK_CUR);
            break;

        }
        free(tag);
        free(size);
       
    }

    //copy remaining data
    char ch;
    while((ch=fgetc(mp3Tag->fptr_mp3))!=EOF)
    {
        fputc(ch,mp3Tag->fptr_temp_mp3);
    }

    return e_success;
    
}

Status copy_mp3_header(FILE *fptr_src, FILE *fptr_dest)
{
    fseek(fptr_src,0,SEEK_SET);
    fseek(fptr_dest,0,SEEK_SET);

    char *buffer=malloc(10*sizeof(char));

    fread(buffer,1,10,fptr_src);
    fwrite(buffer,1,10,fptr_dest);


    free(buffer);
    return e_success;
}

