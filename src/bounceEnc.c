// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

// It takes 2
unsigned char *bounce_encrypt(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                              unsigned char *output) {
  bounce_encrypt_pass_rl(msg, msgLen, key, output);
  bounce_encrypt_pass_lr(output, msgLen, key, output);
  return output;
}

// Right to left encryption pass
unsigned char *bounce_encrypt_pass_rl(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                                      unsigned char *output) {
  // Last output byte is input ^ (random byte)
  output[msgLen - 1] = msg[msgLen - 1] ^ key[255];
  // Main encryption loop
  for (unsigned int i = msgLen - 2; i + 1 >= 1; i--) {
    // Each iteration uses previously computed output byte as random byte index
    // Add i into the index to destroy bounce cycles
    output[i] = msg[i] ^ ((key[output[i + 1]] + i) % 256);
  }
  return output;
}

// Left to right encryption pass
unsigned char *bounce_encrypt_pass_lr(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                                      unsigned char *output) {
  // First output byte is input ^ (random byte)
  output[0] = msg[0] ^ key[0];
  // Main encryption loop
  for (unsigned int i = 1; i <= msgLen - 1; i++) {
    // Each iteration uses previously computed output byte as random byte index
    // Add i into the index to destroy bounce cycles
    output[i] = msg[i] ^ ((key[output[i - 1]] + i) % 256);
  }
  return output;
}
