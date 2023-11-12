# Bounce Encryption Tool

Bounce is an open-source encryption program designed around an extremely simple algorithm. Bounce is meant for academic research as well as a showcase of a reversable hash that will be used in the SMS programming language. It is a simple command line tool for POSIX environments. This program allows you to generate a 256-byte key and use it to encrypt or decrypt files. Bounce is released under the BSD 2-Clause License.

## Key Features

- Powerful encryption: Guessing a 256 byte key is much harder than guessing a 256 bit key. It's 256 times
  harder.
- FAST. The algorithm uses byte operations, which computers deal with better than bitwise operations.
- Ease of use: Generate a 256-byte key and encrypt or decrypt files easily.
- Efficient for messages as small as 1 byte, and works for arbitrarily large files.
- Only requires ~1Kb memory. (not multithreaded, does 256 bytes at a time).
- An understandable algorithm. Nothing unnecessary, just bouncing.
- Accepts data pipes for input and output.

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
  
4. Compile bounce
   ```shell
   sudo make install

## Help , straight form the CLI
```shell
    
    

Binary release soon!



