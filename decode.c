#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "decode.h"

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo){
    if (argv[2] == NULL || strstr(argv[2], ".bmp") == NULL){  //Here the argv[2] equals to null not file provided or if given then .bmp file or not to check
        return d_failure;
    }
    strcpy(decInfo->src_image_fname, argv[2]);   //Here the above argv[2] is correct then that file name should be stote in that src_image_fname

    if (argv[3]){
       for(int i=0;argv[3][i]!='\0';i++){
        if(argv[3][i]=='.'){
            argv[3][i]='\0';
          break;
        }
       }
         strcpy(decInfo->dest_fname, argv[3]); //Here the argv[3] the filename given the data is copy dest_fname
    }else{
        strcpy(decInfo->dest_fname, "output"); ///Here the argv[3] the filename is not given then the default filename will be store in this dest_fname
    }
    return d_success;   //The above all data is correct it will continue
}

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo){
    printf("INFO: ## Decoding Procedure Started ##\n");
    printf("INFO: Opening reqired files\nINFo: Opened Stego.bmp\n");
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r"); //Here we can open the file stego file in read mode the we can decode the dats
    if (!decInfo->fptr_src_image){   //checks the stego image structure contains or not in decInfo
        return d_failure;
    }
    /* Skip bmp image header */
    if (skip_bmp_header(decInfo->fptr_src_image) == d_failure){
        printf("Header is not skipped correctly\n");
        return d_failure;
    }
    /* Decode Store Magic String Size */
    printf("INFO: Decoding Magic String signature\n");
    if (decode_magic_string_size(decInfo) == d_failure){
        printf("Unable to decode size of magic string\n");
        return d_failure;
    }
    /* Decode Store Magic String */
    if (decode_magic_string(decInfo) == d_failure){
        printf("Unable to decode magic string\n");
        return d_failure;
    }
    printf("INFO: Done\n");
    /*Decode extension size*/
    printf("INFO: Decoding Ouput file Extension\n");
    if (decode_secret_file_extn_size(decInfo) == d_failure){
        printf("Unable to decode size of extension\n");
        return d_failure;
    }
    /* Decode secret file extenstion */
    if (decode_secret_file_extn(decInfo) == d_failure){
        printf("Unable to decode extension\n");
        return d_failure;
    } 
    printf("INFO: Done\n");
    /* Decode secret file size */
    int file_size;
    printf("INFO: Decoding Output.txt File size\n");
    if(decode_secret_file_size(&file_size, decInfo) == d_failure){
        printf("Unable to decode size of secret file\n");
        return d_failure;
    }
    printf("INFO: Done\n");
    /* Decode secret file data*/
    // printf("Size of file : %d\n", file_size);
    printf("INFO: Decoding Output.txt File Data\n");
    if(decode_secret_file_data(file_size, decInfo) == d_failure){
        printf("Unable to decode secret file data\n");
        return d_failure;
    }
    printf("INFO: Done\n");
    printf("##  Decoding Done Successfully  ##\n");
    return d_success;
}

/* Skip BMP header */
Status skip_bmp_header(FILE* fptr){
    fseek(fptr, 54, SEEK_SET); //Here the file pointer to moves the 54 th byte in that stego file
    return d_success;
}

/* Decode magic string size */
Status decode_magic_string_size(DecodeInfo *decInfo){
    char buffer[32];  //Here we can declare one array size of 32 bytes
    fread(buffer, 1, 32, decInfo->fptr_src_image); //32 bits (4 bytes) are typically used to store an integer.Since each byte stores 1 bit in its LSB, you need 32 bytes to decode a 32-bit integer.
    decInfo->magic_size = decode_size_from_lsb(buffer);  //extracts the each of the 32 bytes and rearrange the size of the magic string  and stores in magic_size
    if(decInfo->magic_size == 0){  //If not found the magic string or fail the decode it returns
        return d_failure;
    }
    return d_success;
}

/* Decode magic string */
Status decode_magic_string(DecodeInfo *decInfo){
   char buffer[8], magic_string[decInfo->magic_size + 1];//Here one buffer used for read from the image and magic_string store after decode then we have already the magic string size 
    for(int i=0; i<decInfo->magic_size; i++){  //Here the loop runs the size times of magic string
        fread(buffer, 1, 8, decInfo->fptr_src_image);//It extracts the 8bytes of the image  1 character = 8 bits 1 bit stored in each byte’s LSB So, 8 image bytes → 1 character
        magic_string[i] = decode_byte_from_lsb(buffer);//Here the data will decode byte from lsb then it will be store in the magic_string
    }
    magic_string[decInfo->magic_size] = '\0';  //After magic string decode then last we need add the null character
    char user_input[10];  //Here we can take the magic string input from user
    printf("Enter your magic string: ");
    scanf("%s",user_input); //user input

    if(strcmp(magic_string, user_input)){ //Here we can cross check the decode magic string and user input magic string are equal or not
        printf("Magic string unmatched\n");//Doesnot match printing this
        return d_failure;
    }

    return d_success;
}

/* Decode secret file extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo){
    char buffer[32];//Here we can take 32 byte store variable one extracts bit by bit like 32 bytes 
    fread(buffer, 1, 32, decInfo->fptr_src_image);//Here the 32 bytes of data read from the stego image
    decInfo->size_dest_file_extn = decode_size_from_lsb(buffer);//Its extracts the 32 bytes of data and rearrange the secret file extension it will be store in this size_dest_file_extn
    return d_success;
}

/* Decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo){
    char buffer[8], file_extn[decInfo->size_dest_file_extn+1];//Here we need one varible the extesion data will store file_extn +1 to check null pointer also 
    for(int i=0; i<decInfo->size_dest_file_extn; i++){  //Here loop will run the size times of extension
        fread(buffer, 1, 8, decInfo->fptr_src_image);//It will extracts the data from the lsb 
        file_extn[i] = decode_byte_from_lsb(buffer);//Extracts the data from lsb one character and stores in the file_extn
    }
    if(file_extn[0] != '.') {  //Here we can the check the extension with . or not
        printf("Failed to decode extension\n");//here decode will fail it will be print
        return d_failure;
    }

    int length = strlen(decInfo->dest_fname),i;//It will fecth the current file length
    for(i=0; i<decInfo->size_dest_file_extn; i++){
        decInfo->dest_fname[length+i] = file_extn[i];//it will append the extension of decode
    }
    decInfo->dest_fname[length+i] = '\0';//It will store the last character is null character
    //open output file
    decInfo->fptr_dest_file = fopen(decInfo->dest_fname, "w");//Opens the output file in write mode
    //Error handling
    if(decInfo->fptr_dest_file == NULL){//Here check the file is open or not
        perror("fopen");
        fprintf(stderr, "Uable to opening destinatoin file %s\n", decInfo->dest_fname);
        return d_failure;
    }
    printf("Extension : %s\n", decInfo->dest_fname);//here success with the extension
    return d_success;
}

/* Decode secret file size */
Status decode_secret_file_size(int *file_size, DecodeInfo *decInfo){
    char buffer[32];    //32 bytes buffer to store the encode size of data
    fread(buffer, 1, 32, decInfo->fptr_src_image);//read the 32 bytes of data from the stego image it will contain the hidden the data

    *file_size = decode_size_from_lsb(buffer); //it extracts the 32 bytes and give the size of the file and stored file_size
    decInfo->size_secret_file = *file_size;

    if (*file_size <= 0){ //Check the valid size or not or decode also checks
        return d_failure;
    }
    // printf("Size of file : %d\n", *file_size);//it will prints the decode file size
    return d_success;
}


/* Decode secret file data */
Status decode_secret_file_data(int file_size, DecodeInfo *decInfo){
    char buffer[8], ch; //Here we can take the buffer to store 8 bytes of data and one local variable

    for (int i = 0; i < file_size; i++){  //Here loop will run the file size times
        fread(buffer, 1, 8, decInfo->fptr_src_image);//read the 8 bytes of data from the stegoimage
        ch = decode_byte_from_lsb(buffer);//After extracts the data of 8 bytes stores in the ch variable
        fputc(ch, decInfo->fptr_dest_file);//And writes the charcter by character in the output file
    }
    return d_success;
}


/* Decode byte from LSB */
char decode_byte_from_lsb(char *image_buffer){
    char data = 0;   //Here starts with all bits become Zero
    for (int i = 0; i < 8; i++){   //Here the loop starts with o to 8 times run it will extract byte by byte
        data = (data << 1) | (image_buffer[i] & 1);  // (image_buffer[i] & 1)  it will become 1 masks all bits except the LSB
    }                                 // (data << 1) make space from the next bit
    return data;    //Decode data will be return 
}

/* Decode size from LSB */    //Image bytes → LSB bits → shift & combine → original data
int decode_size_from_lsb(char *image_buffer){
    int size = 0;     //start with 0 all bits 
    for (int i = 0; i < 32; i++){  //Loop runs 32 times then extracts 32 bits
        size = (size << 1) | (image_buffer[i] & 1);// (image_buffer[i] & 1)  it will become 1 masks all bits except the LSB
    }                                // (size << 1) make space from the next bit
    return size;    //Decode size will be return 
}
