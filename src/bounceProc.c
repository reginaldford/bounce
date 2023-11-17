#include "bounce.h"
#define SLL sizeof(long long)

// Applies CBC and Bounce Algorithm for an inFile
// Outputs to outFile
void bounceProcess(FILE *inFile, FILE *outFile, unsigned char *key, bool decryptFlag) {
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
        for (short i = 0; i < 256 / SLL; i++)
          ((long long *)buffer)[i] ^= ((long long *)xBuffer)[i];
        // Encrypt
        bounce_encrypt(buffer, bytes_read, key, output);
        // Save to xBuffer
        memcpy(xBuffer, output, 256);
      } else { // Decryption case
        // Process data before CBC
        bounce_decrypt(buffer, bytes_read, key, output);
        // CBC
        for (short i = 0; i < 256 / SLL; i++)
          ((long long *)output)[i] ^= ((long long *)xBuffer)[i];
        // Save buffer to xBuffer
        memcpy(xBuffer, buffer, 256);
      }
      fwrite(output, bytes_read, 1, outFile);
      // Load more data in the buffer
      bytes_read = fread(buffer, 1, 256, inFile);
    } while (bytes_read > 0);
  else
    printf("Input file is empty?\n");
}
