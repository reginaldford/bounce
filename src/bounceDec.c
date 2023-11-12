// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

// It takes 2
unsigned char *bounce_decrypt(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                              unsigned char *output) {
  unsigned char buffer[256];
  bounce_decrypt_pass_lr(msg, msgLen, key, buffer);
  bounce_decrypt_pass_rl(buffer, msgLen, key, output);
  return output;
}

// Right to left decryption pass
unsigned char *bounce_decrypt_pass_rl(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                                      unsigned char *output) {
  // Decrypt last byte with unchanged key
  output[msgLen - 1] = msg[msgLen - 1] ^ key[255];
  // Main decryption loop
  for (unsigned int i = msgLen - 2; i + 1 >= 1; i--) {
    // Each iteration uses previously computed output byte as random byte index
    output[i] = msg[i] ^ ((key[msg[i + 1]] + i) % 256);
  }
  return output;
}

// Left to right decryption pass
unsigned char *bounce_decrypt_pass_lr(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                                      unsigned char *output) {
  output[0] = msg[0] ^ key[0];
  for (unsigned int i = 1; i <= msgLen - 1; i++) {
    output[i] = msg[i] ^ ((key[msg[i - 1]] + i) % 256);
  }
  return output;
}
