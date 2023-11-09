// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

// It takes 2
unsigned char *bounce_encrypt(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                              unsigned char *output) {
  unsigned char *buffer = malloc(msgLen);
  bounce_encrypt_pass(msg, msgLen, key, buffer);
  bounce_encrypt_pass(buffer, msgLen, key, output);
  free(buffer);
  return output;
}

// Each write the output in reverse
unsigned char *bounce_encrypt_pass(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                                   unsigned char *output) {
  // mutable key copy
  unsigned char mKey[256];
  for (int i = 0; i < 256; i++)
    mKey[i] = key[i];
  // Sum the msg (exclude last byte) and use sum as an index for random byte
  unsigned int sum = 0;
  for (int i = 0; i <= msgLen - 2; i++)
    sum += msg[i];
  // Last output byte is input ^ (random byte)
  output[msgLen - 1] = msg[msgLen - 1] ^ key[sum % 256];

  // Main encryption loop
  for (int i = msgLen - 2; i >= 0; i--) {
    // Each iteration uses previous output byte as random byte index
    output[i] = msg[i] ^ mKey[output[i + 1]];
    // Key is mutated as we go
    mKey[output[i + 1]] = (mKey[mKey[output[i + 1]]] + i) % 256;
  }
  return output;
}
