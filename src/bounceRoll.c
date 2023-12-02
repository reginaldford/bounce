// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

// Roll the information from right to left
void bounce_roll_rl(unsigned char *msg, unsigned int msgLen, unsigned char *output) {
  unsigned int sum = 0;
  for (int i = msgLen - 1; i >= 0; i--) {
    output[i] = (msg[i] ^ ((unsigned char *)&sum)[0]);
    for (int j = 1; j < sizeof(int); j++)
      output[i] ^= ((unsigned char *)&sum)[j];
    sum = sum * sum - msg[i] * msg[i] - i;
  }
}

// Roll the information from left to right
void bounce_roll_lr(unsigned char *msg, unsigned int msgLen, unsigned char *output) {
  unsigned int sum = 0;
  for (int i = 0; i < msgLen; i++) {
    output[i] = (msg[i] ^ ((unsigned char *)&sum)[0]);
    for (int j = 1; j < sizeof(int); j++)
      output[i] ^= ((unsigned char *)&sum)[j];
    sum = sum * sum - msg[i] * msg[i] - i;
  }
}

// Unroll the information from right to left
void bounce_unroll_rl(unsigned char *msg, unsigned int msgLen, unsigned char *output) {
  unsigned int sum = 0;
  for (int i = msgLen - 1; i >= 0; i--) {
    output[i] = (msg[i] ^ ((unsigned char *)&sum)[0]);
    for (int j = 1; j < sizeof(int); j++)
      output[i] ^= ((unsigned char *)&sum)[j];
    sum = sum * sum - output[i] * output[i] - i;
  }
}

// Unroll the information from left to right
void bounce_unroll_lr(unsigned char *msg, unsigned int msgLen, unsigned char *output) {
  unsigned int sum = 0;
  for (int i = 0; i < msgLen; i++) {
    output[i] = (msg[i] ^ ((unsigned char *)&sum)[0]);
    for (int j = 1; j < sizeof(int); j++)
      output[i] ^= ((unsigned char *)&sum)[j];
    sum = sum * sum - output[i] * output[i] - i;
  }
}
