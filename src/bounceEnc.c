// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

// Roll, then encrypt pass
unsigned char *bounce_encrypt(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                              unsigned int keySum1, unsigned int keySum2, unsigned char *output) {
  unsigned char buffer[msgLen];
  bounce_roll(msg, msgLen, buffer, keySum1, keySum2);
  bounce_encrypt_pass(buffer, msgLen, key, output);
  return output;
}

// Encryption pass
unsigned char *bounce_encrypt_pass(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                                   unsigned char *output) {
  // First output byte is input ^ (random byte)
  output[0] = msg[0] ^ key[0] ^ key[255];
  // Main encryption loop
  for (unsigned int i = 1; i + 1 <= msgLen; i++) {
    // Each iteration uses previously computed output byte as random byte index
    output[i] = msg[i] ^ (key[output[i - 1]] + key[i]);
  }
  return output;
}
