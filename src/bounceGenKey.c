// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

void bounceGenKey(FILE *fp) {
  const int     num_bytes = 256;
  unsigned char random_bytes[num_bytes];
  for (int i = 0; i < num_bytes; i++) {
    random_bytes[i] = (unsigned char)arc4random();
  }
  for (int i = 0; i < num_bytes; i++) {
    putc(random_bytes[i], fp);
  }
}
