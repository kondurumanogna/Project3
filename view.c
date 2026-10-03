#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"type.h"
#include"view.h"

Status open_files(Mp3tag *mp3Tag)
{
    mp3Tag->fptr_mp3=fopen(mp3Tag->mp3_filename,"rb");

    if( mp3Tag->fptr_mp3==NULL)
    {
        printf("Error : Unable to open MP3 file\n");
        return e_failure;
    }

    return e_success;
}
Status check_signature(Mp3tag *mp3Tag)
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

Status validate_args(char *argv[],Mp3tag *mp3Tag)
{
    int len=strlen(argv[2]);
    if(len<4)
    {
        printf("Entered INVALID EXTENSION FILE\n");
        return e_failure;
    }
    //mp3.mpeg file
    if(argv[2][len-4]!='.')
    {
        printf("Enter Extension of the file\n");
        printf("Usage : ./a.out -v mp3filename.mp3/mpeg\n");
        return e_failure;
    }
    if(argv[2][len-3]!='m')
    {
        printf("Enter Extension of the file correctly\nThere should be one 'm' in the extension\n");
        printf("Usage : ./a.out -v mp3filename.mp3/mpeg\n");
        return e_failure;
    }
    if(argv[2][len-2]!='p')
    {
        printf("Enter Extension of the file correctly\nThere should be one 'p' in the extension\n");
        printf("Usage : ./a.out -v mp3filename.mp3/mpeg\n");
        return e_failure;
    }
    if(argv[2][len-1]!='3')
    {
        printf("Enter Extension of the file correctly\nThere should be one '3' in the extension\n");
        printf("Usage : ./a.out -v mp3filename.mp3/mpeg\n");
        return e_failure;
    }

   
    mp3Tag->mp3_filename=argv[2];

    //read/open file
    if(open_files(mp3Tag)==e_failure)
    {
        printf("Error While opening files\n");
        return e_failure;
    }

    //header check -> ID3
    if(check_signature(mp3Tag)==e_failure)
    {
        printf("Error : Does not have MP3 Signature byte\n");
        return e_failure;
    }

    return e_success;
}

Status execute_view(Mp3tag *mp3Tag)
{
    for(int i=0;i<6;i++)
    {
        //reading tag
        char *tag=malloc(5*sizeof(char));
        fread(tag,1,4,mp3Tag->fptr_mp3);
        tag[4]='\0';
  

        char *size=malloc(4*sizeof(char));
        fread(size,1,4,mp3Tag->fptr_mp3);
        

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

        //skip 3 bytes (2->flag/1->'\0')
        fseek(mp3Tag->fptr_mp3,3,SEEK_CUR);

        char *buffer=malloc((Size.value)*sizeof(char));
        fread(buffer,1,Size.value-1,mp3Tag->fptr_mp3);
        buffer[Size.value-1]='\0';

        if(strcmp(tag,"TIT2")==0 || strcmp(tag,"TPE1")==0 || strcmp(tag,"TALB")==0 || strcmp(tag,"TYER")==0 || strcmp(tag,"TCON")==0 || strcmp(tag,"COMM")==0 )
        {
            printf("TAG %d : %s\n",i,tag);
            printf("Content : %s\n\n",buffer);
        }
       free(tag);
       free(size);
       free(buffer);
    }
        
    return e_success;
}