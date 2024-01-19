// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

// There are 4 functions composed of these 2 macros, only differing on the last line
// So the last line is the parameter to the macro
#define ROLL_MACRO_LR(sum_update)                                                                  \
  unsigned int sum = initState;                                                                    \
  for (unsigned int i = 0; i < msgLen; i++) {                                                      \
    output[i] = (msg[i] ^ ((uint8_t *)&sum)[0]);                                                   \
    for (unsigned int j = 1; j < sizeof(int); j++)                                                 \
      output[i] ^= ((uint8_t *)&sum)[j];                                                           \
    sum = sum_update;                                                                              \
  }

#define ROLL_MACRO_RL(sum_update)                                                                  \
  unsigned int sum = initState;                                                                    \
  for (unsigned int i = msgLen - 1; i + 1 > 0; i--) {                                              \
    output[i] = (msg[i] ^ ((uint8_t *)&sum)[0]);                                                   \
    for (unsigned int j = 1; j < sizeof(int); j++)                                                 \
      output[i] ^= ((uint8_t *)&sum)[j];                                                           \
    sum = sum_update;                                                                              \
  }

// Rolling spreads dependency of every byte to every other byte
void bounce_roll(uint8_t *msg, unsigned int msgLen, uint8_t *output, unsigned int keySum1,
                 unsigned int keySum2) {
  static uint8_t buffer[256];
  bounce_roll_lr(msg, msgLen, buffer, keySum1);
  bounce_roll_rl(buffer, msgLen, output, keySum2);
}

// Undoes rolling
void bounce_unroll(uint8_t *msg, unsigned int msgLen, uint8_t *output, unsigned int keySum1,
                   unsigned int keySum2) {
  static uint8_t buffer[256];
  bounce_unroll_rl(msg, msgLen, buffer, keySum2);
  bounce_unroll_lr(buffer, msgLen, output, keySum1);
}

// Roll the information from left to right
void bounce_roll_lr(uint8_t *msg, unsigned int msgLen, uint8_t *output, unsigned int initState) {
  ROLL_MACRO_LR(sum + SQ(SQ(msg[i])) + i);
}

// Roll the information from right to left
void bounce_roll_rl(uint8_t *msg, unsigned int msgLen, uint8_t *output, unsigned int initState) {
  ROLL_MACRO_RL(sum + SQ(SQ(msg[i])) + i);
}

// Unroll the information from left to right
void bounce_unroll_lr(uint8_t *msg, unsigned int msgLen, uint8_t *output, unsigned int initState) {
  ROLL_MACRO_LR(sum + SQ(SQ(output[i])) + i);
}

// Unroll the information from right to left
void bounce_unroll_rl(uint8_t *msg, unsigned int msgLen, uint8_t *output, unsigned int initState) {
  ROLL_MACRO_RL(sum + SQ(SQ(output[i])) + i);
}
