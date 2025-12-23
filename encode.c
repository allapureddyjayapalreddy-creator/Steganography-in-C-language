#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

uint get_file_size(FILE *fptr)
{
    // Find the size of secret file data
    if (fptr == NULL)
        return 0;

    fseek(fptr, 0, SEEK_END);       // move to end
    long size = ftell(fptr);        // get position = file size

    return (uint)size;
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if(strstr(argv[2], ".bmp") == NULL){
         return e_failure;
    }else{
        if(strcmp(strstr(argv[2], ".bmp"),".bmp")){
            return e_failure;
        }
    }
    encInfo->src_image_fname = argv[2];
     // Here start with the base address then we get the address of the string then the 4 the character is null or not check then perform the operation
    // Here the .bmp will be check then the base address will be store that structure

   if(strstr(argv[3], ".txt") == NULL) {
         return e_failure;
    }else{
        if(strcmp(strstr(argv[3], ".txt"),".txt")){
            return e_failure;
        }
    }
    encInfo->secret_fname = argv[3];
     strcpy(encInfo->extn_secret_file, strstr(argv[3], ".txt"));
            // Here start with the base address then we get the address of the string then the 4 the character is null or not check then perform the operation
             // Here the .bmp will be check then the base address will be store that structure
           
    
    if (argv[4] != NULL){   
    if(strstr(argv[4], ".bmp") == NULL){
         return e_failure;
    }else{
        if(strcmp(strstr(argv[4], ".bmp"),".bmp")){
            return e_failure;
        }

    }        // Here start with the base address then we get the address of the string then the 4 the character is null or not check then perform the operation
     
    // Here the .bmp will be check then the base address will be store that structure
        encInfo->stego_image_fname=argv[4];
    } 
    else
    {
        encInfo->stego_image_fname = "stego.bmp";
    }
    return e_success;
}

Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    // image capacity =get image size for bmp
    // size of secret file =get file size()
    // check the img size is greater than (4*8+2*8+4*8=4*8+4*8+10*8)
    printf("INFO: ## Encoding Procedure Started ##\n");
   
    // secret file size
    printf("INFO: Checking for secret.txt size\n");
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    printf("INFO: Done. Not Empty\n");
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
   
    // calculate required capacity
    uint required_capacity = 0;

    required_capacity += 4 * 8;
    required_capacity += 2 * 8;
    required_capacity += sizeof(int) * 8; // extension size
    required_capacity += strlen(encInfo->extn_secret_file) * 8;
    required_capacity += sizeof(int) * 8;     // secret file size
    required_capacity +=  encInfo->size_secret_file * 8; // actual file data
    printf("INFO: Checking for beautiful.bmp capacity to handle secret.txt\n");
    if (encInfo->image_capacity < required_capacity)
    {
        return e_failure;
    }
    printf("INFO: Done.Found ok \n");
    return e_success;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    printf("INFO: Copying Image Header\n");
    // fseek(fptr_src_image,,SEEK_SET) //we get 18th offset as width
    fseek(fptr_src_image, 0, SEEK_SET);
    char buffer[54];
    fread(buffer, 1, 54, fptr_src_image);   // Here the read the data from the source file only header part then store in buffer
    fwrite(buffer, 1, 54, fptr_dest_image); // here to write the data in destination file only header part

    // validation
    if (ftell(fptr_dest_image) != 54)
    {
        return e_failure;
    }
    return e_success;
}

Status encode_magic_string_size(int size, EncodeInfo *encInfo)
{
    char buffer[32]; // Take a character buffer
    // for (int i = 0; i < 32; i++)
    // { // In buffer we have 32 bytes have store it in
    //     buffer[i] = encInfo->fptr_src_image
    // }

    fread(buffer, 1, 32, encInfo->fptr_src_image);
    if (encode_size_to_lsb(size, buffer) == e_failure)
    {
        return e_failure;
    }
    fwrite(buffer, 1, 32, encInfo->fptr_stego_image);
    return e_success;
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    char buffer[8];
    int size = strlen(magic_string);
    for (int i = 0; i < size; i++)
    {
        // for (int i = 0; i < 8; i++)
        // {
        //     buffer[j] = encInfo->fptr_src_image;
        // }
        fread(buffer, 1, 8, encInfo->fptr_src_image);
        if (encode_byte_to_lsb(magic_string[i], buffer) == e_failure)
        {
            return e_failure;
        }
        fwrite(buffer, 1, 8, encInfo->fptr_stego_image);
    }
    return e_success;
}
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
    char buffer[32]; // Take a character buffer
    // for (int i = 0; i < 32; i++)
    // { // In buffer we have 32 bytes have store it in
    //     buffer[i] = encInfo->fptr_src_image
    // }
    fread(buffer, 1, 32, encInfo->fptr_src_image);
    if (encode_size_to_lsb(size, buffer) == e_failure)
    {
        return e_failure;
    }
    fwrite(buffer, 1, 32, encInfo->fptr_stego_image);
    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char buffer[8];
    int size = strlen(file_extn);
    for (int i = 0; i < size; i++)
    {
        fread(buffer, 1, 8, encInfo->fptr_src_image);
        if (encode_byte_to_lsb(file_extn[i], buffer) == e_failure)
        {
            return e_failure;
        }
        fwrite(buffer, 1, 8, encInfo->fptr_stego_image);
    }
    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char buffer[32];

    // Read 32 bytes from source image
    if (fread(buffer, 1, 32, encInfo->fptr_src_image) != 32)
    {
        return e_failure;
    }

    // Encode the file size into  32 bytes
    if (encode_size_to_lsb(file_size, buffer) == e_failure)
    {
        return e_failure;
    }

    // Write the modified data into stego image
    if (fwrite(buffer, 1, 32, encInfo->fptr_stego_image) != 32)
    {
        return e_failure;
    }

    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char buffer[8];
    char ch;
    int size = encInfo->size_secret_file;
    fseek(encInfo->fptr_secret,0,SEEK_SET);
    for (int i = 0; i < size; i++)
    {
        fread(&ch, 1, 1, encInfo->fptr_secret);
        fread(buffer, 1, 8, encInfo->fptr_src_image);
        if (encode_byte_to_lsb(ch, buffer) == e_failure)
        {
            return e_failure;
        }
        fwrite(buffer, 1, 8, encInfo->fptr_stego_image);
    }
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    int ch;
    while ((ch = fgetc(fptr_src)) != EOF)
    {
        fputc(ch, fptr_dest);
    }
    //  printf("%ld, %ld",ftell(fptr_src), ftell(fptr_dest));
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    /*Get the bit from the 31st position
      clear the lsb of the imagebuffer 0th postion
      move the get bit to the lsb
      set the get value and imagebuffer 0th position*/
    for (int i = 7; i >= 0; i--)
    {
        image_buffer[7 - i] = (image_buffer[7 - i] & (~1));
        if ((data >> i) & 1) //  Here we are getting the msb postion then right shift the postion to left position times then store it
        {
            image_buffer[7 - i] = image_buffer[7 - i] | 1;
        }
    }
    return e_success;
}

Status encode_size_to_lsb(int size, char *imageBuffer)
{
    /*Get the bit from the 31st position
    clear the lsb of the imagebuffer 0th postion
    move the get bit to the lsb
    set the get value and imagebuffer 0th position*/
    for (int i = 31; i >= 0; i--)
    {
        imageBuffer[31 - i] = (imageBuffer[31 - i] & (~1)); // Here to encode the size of the magic,file,string
        if ((size >> i) & 1)                                //  Here we are getting the msb postion then right shift the postion to left position times then store it
        {                                                   // Here bit by bit valid or not left shift will be operate
            imageBuffer[31 - i] = imageBuffer[31 - i] | 1;  // bit by bit getting and store in buffer
        }
    }
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    // call open_files function   == e_failure
    printf("INFO: Opening Files required\n");
    if (open_files(encInfo) == e_failure)
    {
        return e_failure; // return e_faliure
    }
    printf("INFO: Opened beautiful.bmp\nINFO: Opened secret.txt\nINFO: Opened Stego.bmp\n INFO: Done\n");
    // call check capacity function
    if (check_capacity(encInfo) == e_failure)
    {
        return e_failure;
    }
    char ch;
    int i=0;
    while((ch = fgetc(encInfo->fptr_secret)) != EOF)
    {
        encInfo->secret_data[i++] = ch;
    }
    
    // call copy the bmp_header
    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        return e_failure;
    }
    printf("INFO: Done\n");
    printf("INFO: Encoding Magic String Signature\n");
    // call encode_magic-string-size
    if (encode_magic_string_size(strlen(MAGIC_STRING), encInfo) == e_failure)
    {
        return e_failure;
    }
    // call encode_magic-string-
    if (encode_magic_string(MAGIC_STRING, encInfo) == e_failure)
    {
        return e_failure;
    }
    printf("INFO: Done\n");
    // call encode_secret_file_extn_size
    if (encode_secret_file_extn_size(strlen(encInfo -> extn_secret_file), encInfo) == e_failure)
    {
        return e_failure;
    }
    printf("INFO: Encoding secret.txt file Extension\n");
    // call encode_secret_file_extn
    if (encode_secret_file_extn(encInfo -> extn_secret_file, encInfo) == e_failure)
    {
        return e_failure;
    }
    printf("INFO: Done\n");
    printf("INFO: Encoding secret.txt file Size\n");
    // call encode_secret_file_size
    if (encode_secret_file_size(encInfo -> size_secret_file, encInfo) == e_failure)
    {
        return e_failure;
    }
    printf("INFO: Done\n");
    // call encode_secret_file_data
    printf("INFO: Encoding secret.txt file Data\n");
    if (encode_secret_file_data(encInfo) == e_failure)
    {
        return e_failure;
    }
    printf("INFO: Done\n");
    // call copy_remaining_img_data
    printf("INFO: Copying Left Over Data\n");
    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        return e_failure;
    }
    printf("INFO: Done\n");
    printf("##  Encoding Done Successfully  ##\n");
    return e_success;
}
