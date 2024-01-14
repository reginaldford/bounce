#include "bounce.h"
#define SLL sizeof(long long)

// Applies CBC and Bounce Algorithm for an inFile
// Outputs to outFile
void bounceProcess(FILE *inFile, FILE *outFile, unsigned char *key, bool decryptFlag) {
  // Calculate and store the keySum
  unsigned int keySum1 = bounceProcKeySum(key);
  unsigned int keySum2 = bounceProcKeySum(key + 128);
  // Invertable Swap table generated from key
  static unsigned char table[256];
  bounceProcSubTable(key, table);
  // Input buffer
  static unsigned char buffer[256];
  // XOR buffer (for CBC)
  static unsigned char xBuffer[256];
  // Output buffer
  static unsigned char output[256];
  // Zero the xBuffer
  memset(xBuffer, 0, 256);
  // Load data in the buffer
  size_t bytes_read = fread(buffer, 1, 256, inFile);
  if (bytes_read)
    do {
      // Encryption case
      if (!decryptFlag) {
        // CBC before encrypting
        for (unsigned short i = 0; i < bytes_read / SLL; i++)
          ((long long *)buffer)[i] ^= ((long long *)xBuffer)[i];
        // Encrypt
        bounce_encrypt(buffer, bytes_read, key, keySum1, keySum2, table, output);
        // Save to xBuffer
        memcpy(xBuffer, output, bytes_read);
      } else { // Decryption case
        // Process data before unCBC
        bounce_decrypt(buffer, bytes_read, key, keySum1, keySum2, table, output);
        // unCBC
        for (unsigned short i = 0; i < bytes_read / SLL; i++)
          ((long long *)output)[i] ^= ((long long *)xBuffer)[i];
        // Save buffer to xBuffer
        memcpy(xBuffer, buffer, bytes_read);
      }
      fwrite(output, bytes_read, 1, outFile);
      // Load more data in the buffer
      bytes_read = fread(buffer, 1, 256, inFile);
    } while (bytes_read > 0);
  else
    printf("Input file is empty?\n");
}

// Read, Evaluate, Print loop mode
void bounceREPL(unsigned char *key, int decryptFlag) {
  unsigned char input[501];
  unsigned char output[501];
  // Calculate and store the keySum
  unsigned int keySum1 = bounceProcKeySum(key);
  unsigned int keySum2 = bounceProcKeySum(key + 128);
  // Invertable Swap table generated from key
  static unsigned char table[256];
  bounceProcSubTable(key, table);
  while (fgets((char *)input, 501, stdin)) {
    // Read user input, removing newline char
    unsigned int len = strlen((char *)input) - 1;
    if (len <= 0)
      continue;
    input[len] = 0; // Terminating without the newline

    // Decryption turns hex into their byte which is expected to be ascii
    if (decryptFlag) {
      // Filter out bytes that are not lc letters or digits
      int filteredLen = 0;
      for (unsigned int i = 0; i < len; i++) {
        // Only keeping lowercase a-f and 0-9
        if ((input[i] <= 102 && input[i] >= 97) || (input[i] >= 48 && input[i] <= 57)) {
          input[filteredLen++] = input[i];
        }
      }
      // After filtering, we should have a non-empty even-length string
      if (filteredLen == 0 || filteredLen % 2 != 0)
        continue;             // completely ignoring this input
      input[filteredLen] = 0; // Terminating the filtered string
      len                = filteredLen;
      unsigned char tmp[500];
      for (unsigned int i = 0; i < len; i++)
        sscanf((char *)&input[2 * i], "%2hhx", &tmp[i]);
      for (unsigned int i = 0; i < len / 2; i++)
        input[i] = tmp[i];
      bounce_decrypt(input, len / 2, key, keySum1, keySum2, table, output);
      for (unsigned int i = 0; i < len / 2; i++)
        printf("%c", output[i]);
    } else { // Encrypting
      bounce_encrypt(input, len, key, keySum1, keySum2, table, output);
      // Print the bytes as 2 ascii chars each, using hex
      for (unsigned int i = 0; i < len; i++)
        printf("%.2x", output[i]);
    }
    printf("\n");
  }
}

// Helper function to get key sum for half of key
// This function is used for initial state in roll
unsigned int bounceProcKeySum(unsigned char *key) {
  unsigned int sum = 0;
  for (int i = 0; i < 128; i++)
    sum += SQ(SQ(key[i]));
  return sum;
}

// Swapping two pairs of a self referencing array
// s.t. array[array[a1]]=a1 and array[array[b1]]=b1 always
void swap(unsigned char *array, unsigned int a1, unsigned int b1) {
  unsigned char a2 = array[a1];
  unsigned char b2 = array[b1];
  array[a1]        = array[b1];
  array[b1]        = a2;
  array[a2]        = array[b2];
  array[b2]        = a1;
}

// Creates a random byte substitution table based on the key.
// s.t. table[table[x]]=x for 0<=x<=255
void bounceProcSubTable(unsigned char *key, unsigned char *table) {
  // This creates a plain, linear, invertable table first
  // Setting table to [ 0, 1, 2, 3....]
  for (int i = 0; i < 256; i++)
    table[i] = -i; // intentional underflow
  // Randomly swapping invertible pairs
  for (int i = 0; i + 1 < 256; i += 2) {
    unsigned char a1 = key[i];
    unsigned char b1 = key[i + 1];
    // If the swap can keep the inversion property, do the swap
    if (!(a1 == b1 || a1 == table[a1] || b1 == table[b1] || table[a1] == table[b1] ||
          a1 == table[b1]))
      swap(table, a1, b1);
  }
}
