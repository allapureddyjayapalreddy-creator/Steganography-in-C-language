#include <stdio.h>
#ifndef DECODE_H
#define DECODE_H
#include "types.h"
#define MAGIC_STRING "#*"

typedef struct _DecodeInfo
{
    FILE *fptr_src_image;   //To get the data from the source image
    FILE *fptr_dest_file;   //To store the data in the destination file 

    char src_image_fname[100];   //To store the source file name
    char dest_fname[100];        //To store the destination file name

    int magic_size;           // To store the magic string size
    int size_dest_file_extn;    //To store the size of the extension file of the destination file
    long size_secret_file;      //To store the secret file size
    long size_dest_file;        //To store the size of the output file

} DecodeInfo;

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Skip bmp image header */
Status skip_bmp_header(FILE* fptr);

/* Store Magic String Size */
Status decode_magic_string_size(DecodeInfo *decInfo);

/* Store Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/*Decode extension size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(int* file_size,DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(int file_size,DecodeInfo *decInfo);

/* Decode a byte into LSB of image data array */
char decode_byte_from_lsb(char *image_buffer);

/* Decode a size to lsb */
int decode_size_from_lsb(char *image_buffer);

#endif
