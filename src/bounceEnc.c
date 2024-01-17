// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

// Roll, then encrypt pass
unsigned char *bounce_encrypt(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                              unsigned int keySum1, unsigned int keySum2, unsigned char *table,
                              unsigned char *output) {
  unsigned char buffer[msgLen];
  bounce_roll(msg, msgLen, buffer, keySum1, keySum2);
  bounce_encrypt_pass(buffer, msgLen, key, table, output);
  return output;
}

// Encryption pass
unsigned char *bounce_encrypt_pass(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                                   unsigned char *table, unsigned char *output) {
  // Recusive bit flipping
  output[0] = bounceRflip(msg[0]);
  // Use substitution table to randomize first byte
  output[0] = table[output[0]];
  // Main encryption loop
  for (unsigned int i = 1; i + 1 <= msgLen; i++) {
    // Bounce equation first
    unsigned char index = msg[i] ^ (key[output[i - 1]] + key[i]);
    // Substitution table second
    output[i] = table[index];
  }
  return output;
}

// Add 128, reverse, add 128
uint8_t bounceRflip(uint8_t byte) {
  // Add 128
  byte += 128;
  // Reverse
  uint8_t rbyte = 0;
  for (int i = 0; i < 8; i++) {
    rbyte = rbyte | (byte & 1);
    if (i < 7) {
      rbyte = rbyte << 1;
      byte  = byte >> 1;
    }
  }
  // Add 128
  rbyte += 128;
  return rbyte;
}
