// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

// Decrypt pass after rolling both ways
unsigned char *bounce_decrypt(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                              unsigned char *output) {
  unsigned char buffer[msgLen];
  bounce_decrypt_pass(msg, msgLen, key, buffer);
  bounce_unroll(buffer, msgLen, output);
  return output;
}

// Left to right decryption pass
unsigned char *bounce_decrypt_pass(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                                   unsigned char *output) {
  // Computing the key sum
  unsigned char keySum = 0;
  for (int i = 0; i < 256; i++)
    keySum += key[i];
  // First otuput byte is input ^ (random byte)
  output[0] = msg[0] ^ key[keySum];
  // Main decryption loop
  for (unsigned int i = 1; i <= msgLen - 1; i++) {
    // Each iteration uses previously computed output byte as random byte index
    output[i] = msg[i] ^ (key[msg[i - 1]] + i);
  }
  return output;
}
