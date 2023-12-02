// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

// Encrypt pass after rolling both ways
unsigned char *bounce_encrypt(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                              unsigned char *output) {
  unsigned char buffer[msgLen];
  bounce_roll(msg, msgLen, buffer);
  bounce_encrypt_pass(buffer, msgLen, key, output);
  return output;
}

// Left to right encryption pass
unsigned char *bounce_encrypt_pass(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                                   unsigned char *output) {
  // Computing the key sum
  unsigned char keySum = 0;
  for (int i = 0; i < 256; i++)
    keySum += key[i];
  // First output byte is input ^ (random byte)
  output[0] = msg[0] ^ key[keySum];
  // Main encryption loop
  for (unsigned int i = 1; i <= msgLen - 1; i++) {
    // Each iteration uses previously computed output byte as random byte index
    output[i] = msg[i] ^ (key[output[i - 1]] + i);
  }
  return output;
}
