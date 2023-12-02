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

// 2's compliment rule to count bits
int bitcount(unsigned char byte) {
  int count;
  for (count = 0; byte; count++)
    byte &= byte - 1;
  return count;
}

// Provide a random byte that has 2 to 6 set bits
unsigned short int goodRandomByte() {
  // Seed the random number generator with the current time
  srand(((unsigned int)-1) & bounceTime());
  unsigned short int output = rand() % 256;
  while (bitcount(output) < 2 || bitcount(output) > 6)
    output = rand() % 256;
  return output;
}

// Ensures that a randomly generated key is good enough
void bounceKeyQualify(unsigned char *key) {
  // The average bitcount cannot be out of range
  // For any chunk of 4 bytes
  for (int i = 0; i < 256 / 4; i++) {
    int sum = 0;
    for (int j = 0; j < 4; j++)
      sum += bitcount(key[4 * i + j]);
    int avg = 0.5 + sum / 4;
    if (avg < 2 || avg > 6)
      for (int j = 0; j < 4; j++)
        key[4 * i + j] ^= goodRandomByte(); // ensures the good avg
  }
  // First and last byte of the key is checked
  if (bitcount(key[0]) < 2 || bitcount(key[0] > 6))
    key[0] = goodRandomByte();
  if (bitcount(key[255]) < 2 || bitcount(key[255] > 6))
    key[255] = goodRandomByte();
  // First and last byte must be different
  while (key[0] == key[255])
    key[255] = goodRandomByte();
}

// Generate 256 random bytes into outputFile
// Starts with /dev/urandom
// Mixes it more just in case
void bounceGenKey(FILE *outputFile) {
  // 256 byte buffer is the key and msg
  unsigned char buffer[256];
  FILE         *randFile;
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
  ((long long *)buffer)[0] ^= bounceTime();
  // Spread time info into key via LR pass
  bounce_encrypt(buffer, 256, buffer, bounceProcKeySum(buffer), buffer);
  // Ensure key quality
  bounceKeyQualify(buffer);
  // Output
  for (int i = 0; i < 256; i++)
    putc(buffer[i], outputFile);
}
