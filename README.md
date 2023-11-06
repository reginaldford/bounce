# Bounce Encryption Tool

Bounce is an open-source encryption program designed around an extremely simple algorithm. It's meant for academic research as well as a showcase of a reversable hash that will be used in the SMS programming language. It is a simple command line tool for POSIX environments. This project allows you to generate a 256-byte key and use it to encrypt or decrypt files. Bounce is released under the BSD 2-Clause License.

## Key Features

- Fairly OK encryption: Bounce uses bytewise XOR operations with a 256-byte key.
- This is meant for speed. It's not reviewed by encryption scientists or anything.
- Ease of use: Generate a 256-byte key and encrypt or decrypt files easily.
- Designed for whole file encryption. Suitable for encrypting files that fit within your computer's free memory.

## Getting Started

To get started with Bounce, follow these simple steps:

1. Clone the repository to your local machine:
   ```shell
   git clone https://github.com/ReginaldFord/bounce.git
  
2. Compile the code. You might need to edit the Makefile for your compiler
   ```shell
   cd bounce
   make
  
3. Consider installing
   ```shell
   sudo make install
  
4. Use bounce
   ```shell
   sudo make install
   bounce -g > myKey.dat
   bounce -k myKey.dat -i msg.txt -o msg.txt.b


Binary release soon!



