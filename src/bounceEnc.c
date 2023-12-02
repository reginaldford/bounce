// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

// Encrypt pass after rolling both ways
unsigned char *bounce_encrypt(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                              unsigned char *output) {
  unsigned char buffer[msgLen];
  unsigned char buffer2[msgLen];
  bounce_roll_lr(msg, msgLen, buffer);
  bounce_roll_rl(buffer, msgLen, buffer2);
  bounce_encrypt_pass(buffer2, msgLen, key, output);
  return output;
}

// Left to right encryption pass
unsigned char *bounce_encrypt_pass(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                                   unsigned char *output) {
  // First output byte is input ^ (random byte)
  output[0] = msg[0] ^ key[0];
  // Main encryption loop
  for (unsigned int i = 1; i <= msgLen - 1; i++) {
    // Each iteration uses previously computed output byte as random byte index
    output[i] = msg[i] ^ ((key[output[i - 1]] + i) % 256);
  }
  return output;
}
