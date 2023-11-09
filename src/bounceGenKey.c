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

// Doing something less deterministic
// By writing and deleting a tmp file
void bounceSpendTime() {
  char *filename = ".__bounce_tmp_file_";
  // Create and open the file
  FILE *fd = fopen(filename, "wb");
  if (fd == NULL) {
    perror("Error creating file");
    return;
  }
  // Using malloc to take time
  char *data = malloc(1024);
  for (int i = 0; i < 1024; i++) {
    data[i] = (char)i;
  }
  // Write the data to the file
  fwrite(data, 1, sizeof(data), fd);
  // Close and delete
  fclose(fd);
  if (unlink(filename) == -1) {
    perror("Error deleting file");
  }
}

// Generate 256 random bytes into fp
// Improves upon cstd randomness
// Writes a 1kb file, 32 times to lose deterministic timing
// XOR clock time into the key during 32 iterations
void bounceGenKey(FILE *fp) {
  // Buffer is large enough for each byte to index back into buffer
  unsigned char random_bytes[256];
  // Seed random generator with time in ms
  struct timeval tv;
  gettimeofday(&tv, NULL);
  unsigned long long milliseconds =
    (unsigned long long)tv.tv_sec * 1000 + (unsigned long long)tv.tv_usec / 1000;
  srand((unsigned)milliseconds);
  // Start with cstd random bytes, seeded with the time
  for (int i = 0; i < 256; i++)
    random_bytes[i] = rand();
  // Spend some time.
  for (int i = 0; i < 32; i++) {
    bounceSpendTime();
    // Xor the time into a different place in the key per iteration
    ((long long *)random_bytes)[i % (256 / sizeof(long long))] ^= bounceTime();
  }
  // Finish with 4 bounce passes
  for (unsigned int i = 0; i < 4; i++)
    bounce_encrypt_pass(random_bytes, 256, random_bytes, random_bytes);
  // Output
  for (int i = 0; i < 256; i++)
    putc(random_bytes[i], fp);
}
