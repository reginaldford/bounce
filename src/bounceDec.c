// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

unsigned char *bounce_decrypt(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                              unsigned char *output) {
  // Keep the original key
  unsigned char origKey[256];
  for (int i = 0; i < 256; i++)
    origKey[i] = key[i];
  // Main decryption loop
  for (int i = 1; i < msgLen; i++) {
    // Each iteration uses previous output byte as random byte index
    output[i] = msg[i] ^ key[msg[i - 1]];
    // Key is mutated as we go
    key[msg[i - 1]] = (key[key[msg[i - 1]]] + i) % 256;
  }
  // Sum the output (exclude first byte) and use sum as an index for the random byte
  int sum = 0;
  for (int i = 1; i < msgLen; i++)
    sum += output[i];
  // Decrypt first byte with origKey
  output[0] = msg[0] ^ origKey[sum % 256];
  return output;
}
