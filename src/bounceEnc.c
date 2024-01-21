// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

// Roll, then encrypt pass
uint8_t *bounce_encrypt(uint8_t *msg, uint32_t msgLen, uint8_t *key, uint32_t keySum1,
                        uint32_t keySum2, uint8_t *table, uint8_t *output) {
  uint8_t buffer[msgLen];
  bounce_roll(msg, msgLen, buffer, keySum1, keySum2);
  bounce_encrypt_pass(buffer, msgLen, key, table, output);
  return output;
}

// Decrypt pass, then unroll
uint8_t *bounce_decrypt(uint8_t *msg, uint32_t msgLen, uint8_t *key, uint32_t keySum1,
                        uint32_t keySum2, uint8_t *table, uint8_t *output) {
  uint8_t buffer[msgLen];
  bounce_decrypt_pass(msg, msgLen, key, table, buffer);
  bounce_unroll(buffer, msgLen, output, keySum1, keySum2);
  return output;
}

// Encryption pass
uint8_t *bounce_encrypt_pass(uint8_t *msg, uint32_t msgLen, uint8_t *key, uint8_t *table,
                             uint8_t *output) {
  // Trade this byte for another psuedo-random byte
  output[0] = bounce_trade_byte(msg[0]);
  // Use substitution table to randomize first byte
  output[0] = table[output[0]];
  // Main encryption loop
  for (uint32_t i = 1; i + 1 <= msgLen; i++) {
    // Bounce equation first
    uint8_t index = msg[i] ^ (key[output[i - 1]] + key[i]);
    // Substitution table second
    output[i] = table[index];
  }
  return output;
}

// Decryption pass
uint8_t *bounce_decrypt_pass(uint8_t *msg, uint32_t msgLen, uint8_t *key, uint8_t *table,
                             uint8_t *output) {
  // Use substitution table to invert the encryption pass
  output[0] = table[msg[0]];
  // Trade this byte for another psuedo-random byte
  output[0] = bounce_trade_byte(output[0]);
  // Main decryption loop
  for (uint32_t i = 1; i <= msgLen - 1; i++) {
    // Substitution table first
    uint8_t msgi = table[msg[i]];
    // Bounce equation second
    output[i] = msgi ^ (key[msg[i - 1]] + key[i]);
  }
  return output;
}

// Add 128, reverse, add 128. f(f(x))=x
uint8_t bounce_trade_byte(uint8_t byte) {
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
