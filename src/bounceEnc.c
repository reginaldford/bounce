// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

// Roll, then encrypt pass
unsigned char *bounce_encrypt(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                              unsigned int keySum1, unsigned int keySum2, unsigned char *table,
                              unsigned char *output) {
  unsigned char buffer[msgLen];
  bounce_roll(msg, msgLen, buffer, keySum1, keySum2);
  bounce_encrypt_pass(buffer, msgLen, key, table, output);
  return output;
}

// Encryption pass
unsigned char *bounce_encrypt_pass(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                                   unsigned char *table, unsigned char *output) {
  // Recusive bit flipping
  output[0] = bounceRflip(msg[0]);
  // Use substitution table to randomize first byte
  output[0] = table[output[0]];
  // Main encryption loop
  for (unsigned int i = 1; i + 1 <= msgLen; i++) {
    // Bounce equation first
    unsigned char index = msg[i] ^ (key[output[i - 1]] + key[i]);
    // Substitution table second
    output[i] = table[index];
  }
  return output;
}

// Recursive flipping of a byte: nibbles, crumbs
// Leave the bits inside crumbs unflipped, else we get full byte reversal
// Note: byte = bounceRflip(bounceRflip(byte))
uint8_t bounceRflip(uint8_t byte) {
  uint8_t crumbs[4];
  uint8_t nibbles[2];
  // Divide the byte into 4 crumbs
  for (int i = 0; i < 4; i++)
    crumbs[i] = (byte >> (2 * i)) & 0x03;
  // Flip the crumbs inside the nibbles
  nibbles[1] = (crumbs[1] << 2) | crumbs[0];
  nibbles[0] = (crumbs[3] << 2) | crumbs[2];
  // Flip the nibbles inside the output byte
  return ((nibbles[1] << 4) | nibbles[0]);
}
