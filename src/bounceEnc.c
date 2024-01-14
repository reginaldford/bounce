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
  // Nibble flip
  output[0] = ((msg[0] & 15) << 4) | ((msg[0] & 240) >> 4);
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
