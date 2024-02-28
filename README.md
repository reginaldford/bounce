# Bounce Encryption Tool

Bounce is an open-source symmetric encryption tool. The algorithm is easy to read. Bounce is for encrypting files of any size and small messages. Bounce is released under the BSD 2-Clause License. Binary executables are available in the releases section.

## Key Features
- Readable code. It's designed to be trustworthy, and without unnecessary steps.
- Powerful encryption: Guessing a 256 byte key is much harder than guessing a 256 bit key.
- Standalone Executable written in standard C relies on no dynamic libraries or static libraries other than glibc.
- Fast. The program does the hard work first: generating a substitution table and key sums.
- Ease of use: Generate a 256-byte key and encrypt or decrypt files in a few commands.
- Efficient for messages as small as 1 byte, and works for large files.
- Only requires ~1Kb memory. (not multithreaded, does 256 bytes at a time).
- An understandable algorithm. Written about as clearly as possible.
- Accepts data pipes for input and output.
- Cypher Block Chaining is always on for files, making it impossible to detect block-size patterns in large messages.
- REPL mode allows chat-style encryption and decryption for copy and paste.
  
## Explanation of the algorithm
### Bounce is defined in terms of Standard C Operations.
- View [bounceEnc.c](src/bounceEnc.c) for the bounce encryption / decryption functions.
- View [bounceRoll.c](src/bounceRoll.c) to see the rolling functions which make all bytes depend on all other bytes.
- View [bounceProc.c](src/bounceProc.c) to see the processes from the top.
- View [bounceTable.c](src/bounceTable.c) to see the process for generating an invertable byte substitution table, such that `table[table[x]]==x`.

## Getting Started

To get started with Bounce, follow these simple steps:

1. Clone the repository to your local machine with the following command:
   ```shell
   git clone https://github.com/ReginaldFord/bounce
2. Compile the code with the following command. You might need to edit the Makefile for your compiler.
   ```shell
   cd bounce
   make
3. Build and Install with this command:
   ```shell
   sudo make install

## Help displayed by **bounce -h**

```shell
    Uses and examples:
    1. Generate a key:
      bounce -g > myKey

    2. Encrypt a file:
      bounce -k myKey -i msg.txt -o msg.txt.b

    3. Decrypt a file (note the -d):
      bounce -k myKey -i msg.txt.b -o msg.txt -d

    Note: Bounce accepts data pipes for input and/or output.
    Omitting -i and/or -o uses a pipe instead of a file:
      echo "secret" | bounce -k k | bounce -k k -d
    4. REPL allows conversion to and from clear text and encrypted hex.
      bounce -k myKey -r
    Adding -d to above command will create a decrypting REPL.
