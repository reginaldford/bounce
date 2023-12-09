# Bounce Encryption Tool

Bounce is an open-source encryption program designed around a simple algorithm. Bounce is meant for academic research as well as a showcase of a reversible hash that will be used in the SMS programming language. It is a simple command line tool for POSIX environments. This program allows you to generate a 256-byte key and use it to encrypt or decrypt files. Bounce is released under the BSD 2-Clause License. Binary executables are available in the releases section.

## Key Features

- Powerful encryption: Guessing a 256 byte key is much harder than guessing a 256 bit key. 
- FAST. The algorithm uses byte operations, which computers deal with better than bitwise operations.
- Ease of use: Generate a 256-byte key and encrypt or decrypt files easily.
- Efficient for messages as small as 1 byte, and works for arbitrarily large files.
- Only requires ~1Kb memory. (not multithreaded, does 256 bytes at a time).
- An understandable algorithm. Nothing unnecessary, just 'bouncing'.
- Accepts data pipes for input and output.
- Cypher Block Chaining is always on, making it impossible to detect block-size patterns in any encrypted message.
- REPL mode allows immediate encrption and decryption to a format safe for copy and paste.
  
## Explanation of the algorithm
- First, the program does a "roll" operation on the message (or 256 bytes of the message), which does not depend on the key, and can easily be undone by anyone who has the unroll function, but this spreads the dependancy of all bytes to all bytes, accomplishing the necessary diffusion for good encryption. This is done in 2 parts, spreading the dependancy from left to right, then from right to left. This algorithm relies on a state machine that has 2^32 states. Since there is a roll from left to right and from right to left, there is less than 1/2^64 chance of state maching coincidentally. A matching state case for two different inputs is also impossible without having the state being wrong for more than 1 byte. Then, there's a second round of rolling, where the differing bytes cause the rest of the message to be completely different.
- Next, the bouncing pass is applied from left to right. The first byte of the message is XOR'd with a byte that is the sum of the key's bytes, as a byte. Then, every byte after the first is XOR'd with the byte from the key with the index equal to the previous byte, interpetted as an integer, with the byte index added to the value.

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
Using -d will create a decrypting REPL
