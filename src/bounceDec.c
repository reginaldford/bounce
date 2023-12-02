// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

// Decrypt pass before unrolling
unsigned char *bounce_decrypt(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                              unsigned char keySum, unsigned char *output) {
  unsigned char buffer[msgLen];
  bounce_decrypt_pass(msg, msgLen, key, keySum, buffer);
  bounce_unroll(buffer, msgLen, output);
  return output;
}

// Left to right decryption pass
unsigned char *bounce_decrypt_pass(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                                   unsigned char keySum, unsigned char *output) {
  // First output byte is input ^ (random byte)
  output[0] = msg[0] ^ key[keySum];
  // Main decryption loop
  for (unsigned int i = 1; i <= msgLen - 1; i++) {
    // Each iteration uses previously computed output byte as random byte index
    output[i] = msg[i] ^ (key[msg[i - 1]] + i);
  }
  return output;
}
