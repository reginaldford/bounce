// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

// There are 4 functions composed of these 2 macros, only differing on the last line
// So the last line is the parameter to the macro
#define ROLL_MACRO_LR(sum_update)                                                                  \
  for (unsigned int i = 0; i < msgLen; i++) {                                                      \
    output[i] = (msg[i] ^ ((unsigned char *)&sum)[0]);                                             \
    for (unsigned int j = 1; j < sizeof(int); j++)                                                 \
      output[i] ^= ((unsigned char *)&sum)[j];                                                     \
    sum = sum_update;                                                                              \
  }

#define ROLL_MACRO_RL(sum_update)                                                                  \
  for (unsigned int i = msgLen - 1; i + 1 > 0; i--) {                                              \
    output[i] = (msg[i] ^ ((unsigned char *)&sum)[0]);                                             \
    for (unsigned int j = 1; j < sizeof(int); j++)                                                 \
      output[i] ^= ((unsigned char *)&sum)[j];                                                     \
    sum = sum_update;                                                                              \
  }

// Rolling spreads dependency of every byte to every other byte
void bounce_roll(unsigned char *msg, unsigned int msgLen, unsigned char *output) {
  unsigned char buffer[msgLen];
  bounce_roll_lr(msg, msgLen, buffer);
  bounce_roll_rl(buffer, msgLen, output);
}

// Undoes rolling
void bounce_unroll(unsigned char *msg, unsigned int msgLen, unsigned char *output) {
  unsigned char buffer[msgLen];
  bounce_unroll_rl(msg, msgLen, buffer);
  bounce_unroll_lr(buffer, msgLen, output);
}

// Roll the information from left to right
void bounce_roll_lr(unsigned char *msg, unsigned int msgLen, unsigned char *output) {
  unsigned int sum = 0;
  ROLL_MACRO_LR(sum + SQ(SQ(msg[i])) + i);
}

// Roll the information from right to left
void bounce_roll_rl(unsigned char *msg, unsigned int msgLen, unsigned char *output) {
  unsigned int sum = 0;
  ROLL_MACRO_RL(sum + SQ(SQ(msg[i])) + i);
}

// Unroll the information from left to right
void bounce_unroll_lr(unsigned char *msg, unsigned int msgLen, unsigned char *output) {
  unsigned int sum = 0;
  ROLL_MACRO_LR(sum + SQ(SQ(output[i])) + i);
}

// Unroll the information from right to left
void bounce_unroll_rl(unsigned char *msg, unsigned int msgLen, unsigned char *output) {
  unsigned int sum = 0;
  ROLL_MACRO_RL(sum + SQ(SQ(output[i])) + i);
}
