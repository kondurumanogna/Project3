#ifndef EDIT_H
#define EDIT_H

#include"type.h"
#include"view.h"


//Check Operation type
Operationtype  check_operation_type(char option);

//open files
Status open_files_edit(Mp3tag *mp3Tag);

//validate args
Status validate_args_edit(char *argv[],Mp3tag *mp3Tag);

//check_signature
Status check_signature_edit(Mp3tag *mp3Tag);

//do_edit
Status do_editing(char *argv[],Mp3tag *mp3Tag);

//copy header
Status copy_mp3_header(FILE *fptr_src, FILE *fptr_dest);





#endif