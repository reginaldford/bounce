// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

// Decrypt pass, then unroll
unsigned char *bounce_decrypt(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                              unsigned int keySum1, unsigned int keySum2, unsigned char *table,
                              unsigned char *output) {
  unsigned char buffer[msgLen];
  bounce_decrypt_pass(msg, msgLen, key, table, buffer);
  bounce_unroll(buffer, msgLen, output, keySum1, keySum2);
  return output;
}

// Decryption pass
unsigned char *bounce_decrypt_pass(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                                   unsigned char *table, unsigned char *output) {
  // Use substitution table to randomize first byte
  output[0] = table[msg[0]];
  // Main decryption loop
  for (unsigned int i = 1; i <= msgLen - 1; i++) {
    // Substitution table first
    unsigned char msgi = table[msg[i]];
    // Bounce equation second
    output[i] = msgi ^ (key[msg[i - 1]] + key[i]);
  }
  return output;
}
