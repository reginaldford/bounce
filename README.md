# Bounce Encryption Tool

Bounce is an open-source symmetric encryption program designed to be easy to read, yet powerful for modern POSIX environments. Bounce is for encrypting files and messages of any size. Bounce is released under the BSD 2-Clause License. Binary executables are available in the releases section.

## Key Features
    - Readable code. It's designed to be trustworthy, and without unnecessary steps.
    - Powerful encryption: Guessing a 256 byte key is much harder than guessing a 256 bit key.
    - Fast. The program does the hard work first: generating a substitution table and key sums.
    - Ease of use: Generate a 256-byte key and encrypt or decrypt files easily.
    - Efficient for messages as small as 1 byte, and works for large files.
    - Only requires ~1Kb memory. (not multithreaded, does 256 bytes at a time).
    - An understandable algorithm. Just C operations on bytes.
    - Accepts data pipes for input and output.
    - Cypher Block Chaining is always on for files, making it impossible to detect block-size patterns in large messages.
    - REPL mode allows chat-style encryption and decryption between formats for copy and paste.
  
## Explanation of the algorithm
### Bounce is defined in terms of standard C operations.
    - View [bounceProc.c](src/bounceProc.c) to see the entire encryption / decryption processes from the top.
    - View [bounceRoll.c](src/bounceRoll.c) to see the rolling procedure which makes all bytes depend on all other bytes.
    - View [bounceEnc.c](src/bounceEnc.c) and [bounceDec.c](src/bounceEnc.c) for the bounce encryption/decryption procedures.

## Getting Started

To get started with Bounce, follow these simple steps:

1. Clone the repository to your local machine:
   ```shell
   git clone https://github.com/ReginaldFord/bounce
2. Compile the code. You might need to edit the Makefile for your compiler
   ```shell
   cd bounce
   make
3. Consider installing
   ```shell
   sudo make install
  

## Help , straight form the CLI

```shell
Uses and examples:
1. Generate a key:
  bounce -g > myKey

2. Encrypt a file:
  bounce -k myKey -i msg.txt -o msg.txt.b

3. Decrypt a file (note the -d):
  bounce -k myKey -i msg.txt.b -o msg.txt -d

Bounce accepts data pipes for input and/or output.
Omitting -i and/or -o uses a pipe instead of a file:
  echo "secret" | bounce -k k | bounce -k k -d

4. REPL allows conversion to and from clear text and encrypted hex.
  bounce -k myKey -r
Adding -d to above command will create a decrypting REPL.

