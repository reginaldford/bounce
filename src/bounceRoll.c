#include "bounce.h"

// Rolling spreads dependency of every byte to every other byte
void bounce_roll(uint8_t *msg, uint32_t msgLen, uint8_t *output, uint32_t keySum1, uint32_t keySum2,
                 uint8_t *table) {
  uint8_t buffer[msgLen];
  bounce_roll_lr(msg, msgLen, buffer, keySum1, table);
  bounce_roll_rl(buffer, msgLen, output, keySum2, table);
}

// Undoes rolling
void bounce_unroll(uint8_t *msg, uint32_t msgLen, uint8_t *output, uint32_t keySum1,
                   uint32_t keySum2, uint8_t *table) {
  uint8_t buffer[msgLen];
  bounce_unroll_rl(msg, msgLen, buffer, keySum2, table);
  bounce_unroll_lr(buffer, msgLen, output, keySum1, table);
}

// Roll the information from left to right
void bounce_roll_lr(uint8_t *msg, uint32_t msgLen, uint8_t *output, uint32_t initState,
                    uint8_t *table) {
  uint32_t sum = initState;
  for (uint32_t i = 0; i < msgLen; i++) {
    output[i] = (msg[i] ^ ((uint8_t *)&sum)[0]);
    for (uint8_t j = 1; j < 4; j++)
      output[i] ^= ((uint8_t *)&sum)[j];
    sum = sum + SQ(SQ(table[msg[i]])) + i;
  }
}

// Roll the information from right to left
void bounce_roll_rl(uint8_t *msg, uint32_t msgLen, uint8_t *output, uint32_t initState,
                    uint8_t *table) {
  uint32_t sum = initState;
  for (uint32_t i = msgLen - 1; i + 1 > 0; i--) {
    output[i] = (msg[i] ^ ((uint8_t *)&sum)[0]);
    for (uint8_t j = 1; j < 4; j++)
      output[i] ^= ((uint8_t *)&sum)[j];
    sum = sum + SQ(SQ(table[msg[i]])) + i;
  }
}

// Unroll the information from left to right
void bounce_unroll_lr(uint8_t *msg, uint32_t msgLen, uint8_t *output, uint32_t initState,
                      uint8_t *table) {
  uint32_t sum = initState;
  for (uint32_t i = 0; i < msgLen; i++) {
    output[i] = (msg[i] ^ ((uint8_t *)&sum)[0]);
    for (uint8_t j = 1; j < 4; j++)
      output[i] ^= ((uint8_t *)&sum)[j];
    sum = sum + SQ(SQ(table[output[i]])) + i;
  }
}

// Unroll the information from right to left
void bounce_unroll_rl(uint8_t *msg, uint32_t msgLen, uint8_t *output, uint32_t initState,
                      uint8_t *table) {
  uint32_t sum = initState;
  for (uint32_t i = msgLen - 1; i + 1 > 0; i--) {
    output[i] = (msg[i] ^ ((uint8_t *)&sum)[0]);
    for (uint8_t j = 1; j < 4; j++)
      output[i] ^= ((uint8_t *)&sum)[j];
    sum = sum + SQ(SQ(table[output[i]])) + i;
  }
}
