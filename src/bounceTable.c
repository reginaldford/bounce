// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info
#include "bounce.h"

// Swapping two pairs of a self referencing table
// s.t. table[table[a1]]=a1 and table[table[b1]]=b1 always
void swap(uint8_t *table, uint32_t a1, uint32_t b1) {
  uint8_t a2 = table[a1];
  uint8_t b2 = table[b1];
  table[a1]  = b2;
  table[b1]  = a2;
  table[a2]  = b1;
  table[b2]  = a1;
}

// If the swap can keep table[ table [ i ] ] = i for all i, do the swap
void maybe_swap(uint8_t *table, uint32_t a1, uint32_t b1) {
  if (!(a1 == b1 || a1 == table[a1] || b1 == table[b1] || table[a1] == table[b1] ||
        a1 == table[b1]))
    swap(table, a1, b1);
}

// Creates a random byte substitution table based on the key.
void bounceProcSubTable(uint8_t *key, uint8_t *table) {
  // Setting table to [ 255 254 ... 0 ]
  // Notice that table[ table [ i ] ] = i for all x
  for (int i = 0; i < 256; i++)
    table[i] = 256 - i;
  // Swapping element pairs of table based on the key
  for (int i = 0; i + 1 < 128; i++) {
    maybe_swap(table, key[2 * i], key[2 * i + 1]);
    maybe_swap(table, key[i], key[255 - i]);
  }
}
