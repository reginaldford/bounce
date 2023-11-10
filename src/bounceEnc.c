// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

// It takes 2
unsigned char *bounce_encrypt(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                              unsigned char *output) {
  unsigned char *buffer = malloc(msgLen);
  bounce_encrypt_pass_rl(msg, msgLen, key, buffer);
  bounce_encrypt_pass_lr(buffer, msgLen, key, output);
  free(buffer);
  return output;
}

// Right to left encryption pass
unsigned char *bounce_encrypt_pass_rl(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                                      unsigned char *output) {
  // Make a mutable key copy
  unsigned char mKey[256];
  for (int i = 0; i < 256; i++)
    mKey[i] = key[i];
  // Last output byte is input ^ (random byte)
  output[msgLen - 1] = msg[msgLen - 1] ^ key[0];
  // Main encryption loop
  for (unsigned int i = msgLen - 2; i + 1 >= 1; i--) {
    // Each iteration uses previous output byte as random byte index
    output[i] = msg[i] ^ mKey[output[i + 1]];
    // Key is mutated as we go
    mKey[output[i + 1]] = (mKey[mKey[output[i + 1]]]) + i % 256;
  }
  return output;
}

// Left to right encryption pass
unsigned char *bounce_encrypt_pass_lr(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                                      unsigned char *output) {
  // Make a mutable key copy
  unsigned char mKey[256];
  for (int i = 0; i < 256; i++)
    mKey[i] = key[i];
  // First output byte is input ^ (random byte)
  output[0] = msg[0] ^ key[0];
  // Main encryption loop
  for (unsigned int i = 1; i <= msgLen - 1; i++) {
    // Each iteration uses previous output byte as random byte index
    output[i] = msg[i] ^ mKey[output[i - 1]];
    // Key is mutated as we go
    mKey[output[i - 1]] = (mKey[mKey[output[i - 1]]]) + i % 256;
  }
  return output;
}
