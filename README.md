# Bounce Encryption Tool

Bounce is an open-source symmetric encryption program designed to be easy to read, yet powerful for modern POSIX environments. Bounce is for encrypting files of any size and small messages. Bounce is released under the BSD 2-Clause License. Binary executables are available in the releases section.

## Key Features
- Readable code. It's designed to be trustworthy, and without unnecessary steps.
- Powerful encryption: Guessing a 256 byte key is much harder than guessing a 256 bit key.
- Standalone Executable written in standard C relies on no dynamic libraries or static libraries other than glibc.
- Fast. The program does the hard work first: generating a substitution table and key sums.
- Ease of use: Generate a 256-byte key and encrypt or decrypt files easily.
- Efficient for messages as small as 1 byte, and works for large files.
- Only requires ~1Kb memory. (not multithreaded, does 256 bytes at a time).
- An understandable algorithm. Just C operations on bytes.
- Accepts data pipes for input and output.
- Cypher Block Chaining is always on for files, making it impossible to detect block-size patterns in large messages.
- REPL mode allows chat-style encryption and decryption between formats for copy and paste.
  
## Explanation of the algorithm
### Bounce is defined in terms of Standard C Operations.
- View [bounceEnc.c](src/bounceEnc.c) for the bounce encryption / decryption procedures.
- View [bounceRoll.c](src/bounceRoll.c) to see the rolling procedure which makes all bytes depend on all other bytes.
- View [bounceProc.c](src/bounceProc.c) to see the entire encryption / decryption processes from the top.
### Long story Short (This is the latest algorithm, release soon)
- Each half of the 256 byte key is used to generate a 4 byte 'key sum'
- The first key sum is used as the inital state to roll from left to right, generating a new message where every byte depends on every byte before in the message.
- The second key sum is used as the inital state to roll from right to left, generating a new message where every byte depends on every byte after it.
- A substitution table is used to trade the first byte for another.
- The bounce pass is done, left to right, and each output byte is substituted through the table
- For decryption,the same algorithm is done in reverse order. The key sums and substitution table can be cached and associated with the key.

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

Bounce accepts data pipes for input and/or output.
Omitting -i and/or -o uses a pipe instead of a file:
  echo "secret" | bounce -k k | bounce -k k -d

4. REPL allows conversion to and from clear text and encrypted hex.
  bounce -k myKey -r
Adding -d to above command will create a decrypting REPL.

