#ifndef VIEW_H
#define VIEW_H

#include"type.h"

typedef struct _Mp3tag
{
    char *mp3_filename;
    FILE *fptr_mp3;

    FILE *fptr_temp_mp3;
    char *temp_filename;

} Mp3tag;

//check operation type 
Operationtype  check_operation_type(char option);

//open files
Status open_files(Mp3tag *mp3Tag);

//validate i/p file
Status validate_args(char *argv[],Mp3tag *mp3Tag);

//check_signature
Status check_signature(Mp3tag *mp3Tag);

//execute_view
Status execute_view(Mp3tag *mp3Tag);




#endif
