// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

// Decrypt pass, then unroll
uint8_t *bounce_decrypt(uint8_t *msg, uint32_t msgLen, uint8_t *key, uint32_t keySum1,
                        uint32_t keySum2, uint8_t *table, uint8_t *output) {
  uint8_t buffer[msgLen];
  bounce_decrypt_pass(msg, msgLen, key, table, buffer);
  bounce_unroll(buffer, msgLen, output, keySum1, keySum2);
  return output;
}

// Decryption pass
uint8_t *bounce_decrypt_pass(uint8_t *msg, uint32_t msgLen, uint8_t *key, uint8_t *table,
                             uint8_t *output) {
  // Use substitution table to invert the encryption pass
  output[0] = table[msg[0]];
  // Recusive bit flipping
  output[0] = bounceRflip(output[0]);
  // Main decryption loop
  for (uint32_t i = 1; i <= msgLen - 1; i++) {
    // Substitution table first
    uint8_t msgi = table[msg[i]];
    // Bounce equation second
    output[i] = msgi ^ (key[msg[i - 1]] + key[i]);
  }
  return output;
}
