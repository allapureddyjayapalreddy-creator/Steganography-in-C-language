📌 Project Description

This project implements Image Steganography using the Least Significant Bit (LSB) technique to securely hide a secret text message inside a BMP image. Developed as part of my learning journey at Emertxe Institute of Technology, the project focuses on low-level data manipulation using C.

🚀 Features

Hides secret text data inside BMP images without visible distortion

Uses LSB technique for secure data embedding

Reads and writes raw image bytes using file handling

Supports command-line arguments

Modular program structure for better maintainability

🛠️ Technologies & Concepts Used

C Programming

File Handling (fopen, fread, fwrite, fclose)

Bitwise Operations

Structures and Pointers

Arrays and Strings

Command Line Argument Handling

Modular Program Architecture

📂 Project Structure
Image_Steganography/
│
├── main.c        # Entry point of the program
├── encode.c      # Encoding logic implementation
├── encode.h      # Header file for encoding functions
├── input.bmp     # Sample input image
├── output.bmp    # Output image with hidden data
├── secret.txt    # Text file to be hidden
└── README.md     # Project documentation

▶️ How to Compile and Run
Encoding a Message

Clone the repository:

https://github.com/allapureddyjayapalreddy-creator/Steganography-in-C-language

Compile the project:

gcc main.c encode.c -o stego


Run the program:

./stego -e input.bmp secret.txt output.bmp
