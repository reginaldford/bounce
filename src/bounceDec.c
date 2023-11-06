// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license
// info

#include "bounce.h"

unsigned char *bounce_decrypt(unsigned char *msg, unsigned int msgLen,
                              unsigned char *key, unsigned char *output) {
  for (int i = 1; i < msgLen; i++)
    output[i] = msg[i] ^ key[msg[i - 1]];
  // Sum the output (exclude first byte) and use sum as an index for the random
  // byte
  int sum = 0;
  for (int i = 1; i < msgLen; i++)
    sum += output[i];
  // First byte calculation
  output[0] = msg[0] ^ key[sum % 256];
  output[msgLen] = 0;
  return output;
}
