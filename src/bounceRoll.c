// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

// There are 4 functions (roll_lr, roll_rl, unroll_lr, unroll_rl),
// that each use 1 these 2 macros, only differing on the last line.
// The last line uses the parameter to the macro (sum_update)
#define ROLL_MACRO_LR(sum_update)                                                                  \
  uint32_t sum = initState;                                                                        \
  for (uint32_t i = 0; i < msgLen; i++) {                                                          \
    output[i] = (msg[i] ^ ((uint8_t *)&sum)[0]);                                                   \
    for (uint32_t j = 1; j < 4; j++)                                                               \
      output[i] ^= ((uint8_t *)&sum)[j];                                                           \
    sum = sum_update;                                                                              \
  }

#define ROLL_MACRO_RL(sum_update)                                                                  \
  uint32_t sum = initState;                                                                        \
  for (uint32_t i = msgLen - 1; i + 1 > 0; i--) {                                                  \
    output[i] = (msg[i] ^ ((uint8_t *)&sum)[0]);                                                   \
    for (uint32_t j = 1; j < 4; j++)                                                               \
      output[i] ^= ((uint8_t *)&sum)[j];                                                           \
    sum = sum_update;                                                                              \
  }

// Rolling spreads dependency of every byte to every other byte
void bounce_roll(uint8_t *msg, uint32_t msgLen, uint8_t *output, uint32_t keySum1,
                 uint32_t keySum2) {
  uint8_t buffer[msgLen];
  bounce_roll_lr(msg, msgLen, buffer, keySum1);
  bounce_roll_rl(buffer, msgLen, output, keySum2);
}

// Undoes rolling
void bounce_unroll(uint8_t *msg, uint32_t msgLen, uint8_t *output, uint32_t keySum1,
                   uint32_t keySum2) {
  uint8_t buffer[msgLen];
  bounce_unroll_rl(msg, msgLen, buffer, keySum2);
  bounce_unroll_lr(buffer, msgLen, output, keySum1);
}

// Roll the information from left to right
void bounce_roll_lr(uint8_t *msg, uint32_t msgLen, uint8_t *output, uint32_t initState) {
  ROLL_MACRO_LR(sum + SQ(SQ(msg[i])) + i);
}

// Roll the information from right to left
void bounce_roll_rl(uint8_t *msg, uint32_t msgLen, uint8_t *output, uint32_t initState) {
  ROLL_MACRO_RL(sum + SQ(SQ(msg[i])) + i);
}

// Unroll the information from left to right
void bounce_unroll_lr(uint8_t *msg, uint32_t msgLen, uint8_t *output, uint32_t initState) {
  ROLL_MACRO_LR(sum + SQ(SQ(output[i])) + i);
}

// Unroll the information from right to left
void bounce_unroll_rl(uint8_t *msg, uint32_t msgLen, uint8_t *output, uint32_t initState) {
  ROLL_MACRO_RL(sum + SQ(SQ(output[i])) + i);
}
