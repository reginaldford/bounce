// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info
#include "bounce.h"

// Applies CBC and Bounce Algorithm for an inFile
// Outputs to outFile
void bounceProcess(FILE *inFile, FILE *outFile, uint8_t *key, bool decryptFlag) {
  // Calculate and store the keySum
  uint32_t keySum1 = bounceProcKeySum(key);
  uint32_t keySum2 = bounceProcKeySum(key + 128);
  // Invertable Swap table generated from key
  static uint8_t table[256];
  bounceTableInit(key, table);
  // Input buffer
  static uint8_t buffer[256];
  // Output buffer
  static uint8_t output[256];
  // XOR buffer (for CBC)
  static uint8_t xBuffer[256];
  // Zero the xBuffer
  memset(xBuffer, 0, 256);
  // Load data in the buffer
  size_t bytes_read = fread(buffer, 1, 256, inFile);
  if (bytes_read)
    do {
      // Encryption case
      if (!decryptFlag) {
        // CBC before encrypting
        for (unsigned short i = 0; i < bytes_read / 8; i++)
          ((uint64_t *)buffer)[i] ^= ((uint64_t *)xBuffer)[i];
        // Encrypt
        bounce_encrypt(buffer, bytes_read, key, keySum1, keySum2, table, output);
        // Save to xBuffer
        memcpy(xBuffer, output, bytes_read);
      } else { // Decryption case
        // Process data before unCBC
        bounce_decrypt(buffer, bytes_read, key, keySum1, keySum2, table, output);
        // unCBC
        for (unsigned short i = 0; i < bytes_read / 8; i++)
          ((uint64_t *)output)[i] ^= ((uint64_t *)xBuffer)[i];
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
void bounceREPL(uint8_t *key, int decryptFlag) {
  uint8_t input[501];
  uint8_t output[501];
  // Calculate and store the keySum
  uint32_t keySum1 = bounceProcKeySum(key);
  uint32_t keySum2 = bounceProcKeySum(key + 128);
  // Invertable Swap table generated from key
  static uint8_t table[256];
  bounceTableInit(key, table);
  while (fgets((char *)input, 501, stdin)) {
    // Read user input, removing newline char
    uint32_t len = strlen((char *)input) - 1;
    if (len <= 0)
      continue;
    input[len] = 0; // Terminating without the newline

    // Decryption turns hex into their byte which is expected to be ascii
    if (decryptFlag) {
      // Filter out bytes that are not lc letters or digits
      int filteredLen = 0;
      for (uint32_t i = 0; i < len; i++) {
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
      uint8_t tmp[500];
      for (uint32_t i = 0; i < len; i++)
        sscanf((char *)&input[2 * i], "%2hhx", &tmp[i]);
      for (uint32_t i = 0; i < len / 2; i++)
        input[i] = tmp[i];
      bounce_decrypt(input, len / 2, key, keySum1, keySum2, table, output);
      for (uint32_t i = 0; i < len / 2; i++)
        printf("%c", output[i]);
    } else { // Encrypting
      bounce_encrypt(input, len, key, keySum1, keySum2, table, output);
      // Print the bytes as 2 ascii chars each, using hex
      for (uint32_t i = 0; i < len; i++)
        printf("%.2x", output[i]);
    }
    printf("\n");
  }
}

// Helper function to get key sum for half of key
// This function is used for initial state in roll
uint32_t bounceProcKeySum(uint8_t *key) {
  uint32_t sum = 0;
  for (int i = 0; i < 128; i++)
    sum += SQ(SQ(key[i]));
  return sum;
}
