#include<stdio.h>
#include<string.h>
#include"view.h"
#include"edit.h"
#include"type.h"

int main(int argc,char *argv[])
{
    Mp3tag mp3Tag;

    if(argc<2)
    {
        printf("USAGE-ERROR HANDLING\n");
        printf("ERROR : ./a.out : INVALID ARGUMENTS\n");
        printf("USAGE:\n");
        printf("To view please pass : ./a.out -v mp3filename\n");
        printf("To Edit please pass : ./a.out -e -c/-a/-A/-m/-y/-c mp3filename\n");
        return e_failure;
    }




    if(check_operation_type(argv[1][1])==e_view)
    {
            if(argc<3)
            {
                printf("Enter valid number of arguments\n");
                printf("Help menu:\n");
                printf("To view please pass : ./a.out -v mp3filename\n");
                return e_failure;

            }
            if(validate_args(argv,&mp3Tag)==e_success)
             {
                  if(execute_view(&mp3Tag)==e_success)
                     {
                          printf("The content in the mp3file is displayed successfully\n");
                     }
                    else
                    {
                           printf("Failed to display the content in the mp3file\n");
                   }
            }
    }
    else if(check_operation_type(argv[1][1])==e_edit)
    {
       if(argc<5)
        {
            printf("Enter valid number of arguments\n");
            printf("Help menu:\n");
            printf("To Edit please pass : ./a.out -e -c/-a/-A/-m/-y/-c mp3filename\n");
            return e_failure;

        }
        if(validate_args_edit(argv,&mp3Tag)==e_success)
        {
            if(do_editing(argv,&mp3Tag)==e_success)
            {
                fclose(mp3Tag.fptr_mp3);
                fclose(mp3Tag.fptr_temp_mp3);

                if(remove(mp3Tag.mp3_filename)!=0)
                {
                    printf("Error : Unable to delete original file\n");
                    return e_failure;
                }
                if(rename(mp3Tag.temp_filename,mp3Tag.mp3_filename)!=0)
                {
                    printf("Error : Unable to rename temp file\n");
                    return e_failure;
                }

                printf("Editing is done Successful\n");
            }
            else
            {
              printf("Failed to edit\n");   
            }
        }
    }
    else if(check_operation_type(argv[1][2])==e_help)
    {
        printf("1. -v  -> to view mp3 file contenst\n");
        printf("2. -e  -> to edit mp3 file contents\n");
        printf("2.1 -t -> to edit song title\n");
        printf("2.2 -a -> to edit artist name\n");
        printf("2.3 -A -> to edit album name\n");
        printf("2.4 -y -> to edit year\n");
        printf("2.5 -m -> to edit content\n");
        printf("2.6 -c -> to edit comment\n");

    }
}

Operationtype  check_operation_type(char option)
{
    if(option=='v') return e_view;
    else if(option=='e') return e_edit;
    else if(option=='h') return e_help;
    else return e_unsupported;
}