// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

// Generate 256 random bytes
void bounceGenKey(FILE *fp) {
  // Each byte of the key is an index into the key (2^8=256)
  const int key_size = 256;
  // Buffer
  unsigned char random_bytes[key_size];
  // High quality randomness
  arc4random_buf(random_bytes, key_size);
  // Output
  for (int i = 0; i < 256; i++)
    putc(random_bytes[i], fp);
}
