#include<stdio.h>
#include<string.h>
#include"view.h"
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
        printf("Edit\n");
    }
    else if(check_operation_type(argv[1][1])==e_help)
    {
        printf("Help\n");
    }
}

Operationtype  check_operation_type(char option)
{
    if(option=='v') return e_view;
    else if(option=='e') return e_edit;
    else if(option=='-') return e_help;
    else return e_unsupported;
}