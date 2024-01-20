// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

// Get the time, as precise as possible
uint64_t bounceTime() {
  struct timespec current_time;
  uint64_t        milliseconds;
  if (clock_gettime(CLOCK_REALTIME, &current_time) == 0) {
    // Calculate the time in milliseconds
    milliseconds = current_time.tv_sec * 1000LL + current_time.tv_nsec / 1000000LL;
  } else {
    perror("clock_gettime");
    exit(1);
  }
  return milliseconds;
}

// Generate 256 random bytes into outputFile
// Starts with /dev/urandom
// Mixes it more just in case
void bounceGenKey(FILE *outputFile) {
  // 256 byte buffer is the key and msg
  uint8_t buffer[256];
  FILE   *randFile;
  // Open /dev/urandom for reading
  randFile = fopen("/dev/urandom", "rb");
  if (randFile == NULL) {
    perror("Error: Cannot open /dev/urandom");
    exit(1);
  }
  // Read 256 bytes of random data into the buffer
  if (fread(buffer, 1, sizeof(buffer), randFile) != sizeof(buffer)) {
    perror("Error: Cannot read from /dev/urandom");
    fclose(randFile);
    exit(1);
  }
  fclose(randFile);
  // XOR the time into the key
  ((uint64_t *)buffer)[0] ^= bounceTime();
  // Mix up the buffer by encrypting it with itself as key
  int     keySum1 = bounceProcKeySum(buffer);
  int     keySum2 = bounceProcKeySum(buffer + 128);
  uint8_t table[256];
  bounceProcSubTable(buffer, table);
  // Using buffer as msg and key
  // The key mutates as we encrypt, bc the key is the output. Irreversible.
  bounce_encrypt(buffer, 256, buffer, keySum1, keySum2, table, buffer);
  // Output
  for (int i = 0; i < 256; i++)
    putc(buffer[i], outputFile);
}
