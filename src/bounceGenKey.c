// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

// Get the time, as precise as possible
unsigned long long bounceTime() {
  struct timespec    current_time;
  unsigned long long milliseconds;
  if (clock_gettime(CLOCK_REALTIME, &current_time) == 0) {
    // Calculate the time in milliseconds
    milliseconds = current_time.tv_sec * 1000LL + current_time.tv_nsec / 1000000LL;
  } else
    perror("clock_gettime");
  return milliseconds;
}

// Generate 256 random bytes into outputFile
// Starts with /dev/urandom
// Mixes it more just in case
void bounceGenKey(FILE *outputFile) {
  // 256 byte buffer is the key and msg
  unsigned char random_bytes[256];
  unsigned char more_bytes[256];
  for (int i = 0; i < 2; i++) {
    FILE *randFile;
    // Open /dev/urandom for reading
    randFile = fopen("/dev/urandom", "rb");
    if (randFile == NULL) {
      perror("Error: Cannot open /dev/urandom");
      exit(1);
    }
    // Read 256 bytes of random data into the random_bytes
    if (fread(more_bytes, 1, sizeof(more_bytes), randFile) != sizeof(more_bytes)) {
      perror("Error: Cannot read from /dev/urandom");
      fclose(randFile);
      exit(1);
    }
    fclose(randFile);
    // XOR more_bytes into the key buffer
    for (int i = 0; i < 256; i++)
      random_bytes[i] ^= more_bytes[255 - i];
  }
  // XOR the time into the key
  ((long long *)random_bytes)[0] ^= bounceTime();
  // Left to right pass
  bounce_encrypt_pass_lr(random_bytes, 256, more_bytes, random_bytes);
  // Change the key
  bounce_encrypt_pass_lr(more_bytes, 256, more_bytes, more_bytes);
  // Right to left pass
  bounce_encrypt_pass_rl(random_bytes, 256, more_bytes, random_bytes);
  // Output
  for (int i = 0; i < 256; i++)
    putc(random_bytes[i], outputFile);
}
