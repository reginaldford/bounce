// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

unsigned char *bounce_encrypt(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                              unsigned char *output) {
  unsigned char rb;
  // Sum the msg (exclude first byte) and use sum as an index for random byte
  int sum = 0;
  for (int i = 1; i < msgLen; i++)
    sum += msg[i];
  // A random byte is selected using the sum
  rb = key[sum % 256];
  // First output byte is input ^ rb
  output[0] = msg[0] ^ rb;
  // Main encryption loop
  for (int i = 1; i < msgLen; i++) {
    // Each iteration uses previous output byte as random byte index
    output[i] = msg[i] ^ key[output[i - 1]];
    // Key is mutated as we go
    key[output[i - 1]] = (key[key[output[i - 1]]] + i) % 256;
  }
  return output;
}
