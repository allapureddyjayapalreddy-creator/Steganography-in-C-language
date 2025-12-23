/*Name   : A.Jayapal Reddy
  Date   : 1/-12-2025
  Description : Project is the Encode the secret message and decode the secret message and store it in the sample out file
                and see the what message is send the in the secret file it will decode then store sample output file
Sample Input : The secret message is 25026F - Adv C
                Encode and Decode Done 
                The above two lines are sample input it is secret mesage that message we can encode 
                The message will be store in the secret file and then after encode the data will be store in the stego.bmp file
Sample Output : The secret message we can decode the ouput will store in the output.txt file */
#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
OperationType check_operation_type(char *);

int main(int argc, char *argv[])
{
    // Step 1 : Check the argc >= 4 true - > step 2
   
    if((argc==4 || argc ==5) &&(check_operation_type(argv[1])==e_encode)){ 
    // Step 2 : Call the check_operation_type(argv[1]) == e_encode )) true - > step 3
    if(argc==4){
        printf("INFO: Output File is Not Mentioned. Stego.bmp file will be created as default\n");
    }
        EncodeInfo enc;
        if(read_and_validate_encode_args(argv,&enc)== e_success){
            printf("INFO: Read and validation is done\n");
             if(do_encoding(&enc)==e_failure){
                return e_failure;
            }
        }else{
            printf("Unsupported file\n");
        } 
    }else if((argc==3 || argc==4) &&(check_operation_type(argv[1])==e_decode)){    
        DecodeInfo dec;
        if(argc==3){
        printf("INFO: Output File is Not Mentioned. Output.txt file will be created as default\n");
        }
        if(read_and_validate_decode_args(argv,&dec)== d_success){
            printf("INFO: Read and validation is done\n");
             if(do_decoding(&dec)==d_failure){
                    return d_failure;
            }
        }else{
            printf("Unsupported file\n");
        } 
    }else{
    printf("d_unsupported file\n");
}
    // Step 3 : Declare structure variable EncodeInfo enc_info
    // Step 4 : Call the read_and_validate_encode_args(argv,&enc_info)== e_success)
    //  true -> Step 5 , false - > terminate the program
    // Step 5 : Call the do_encoding (&encInfo);
   return d_success;
}

OperationType check_operation_type(char *symbol)
{
    // Step 1 : Check whether the symbol is -e or not true - > return e_encode false -> Step 2
    if (strcmp(symbol, "-e") == 0)
        return e_encode;

    if (strcmp(symbol, "-d") == 0)
        return e_decode;

    return e_unsupported;
    // Step 2 : Check whether the symbol is -d or not true - > return e_decode false -> return e_unsupported
}
