#include "bounce.h"
#define SLL sizeof(long long)

// Applies CBC and Bounce Algorithm for an inFile
// Outputs to outFile
void bounceProcess(FILE *inFile, FILE *outFile, unsigned char *key, bool decryptFlag) {
  // Calculate and store the keySum
  unsigned char keySum = bounceProcKeySum(key);
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
        bounce_encrypt(buffer, bytes_read, key, keySum, output);
        // Save to xBuffer
        memcpy(xBuffer, output, bytes_read);
      } else { // Decryption case
        // Process data before unCBC
        bounce_decrypt(buffer, bytes_read, key, keySum, output);
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
  unsigned char keySum = bounceProcKeySum(key);
  while (fgets((char *)input, 501, stdin)) {
    // Ignore blank messages
    if (input[0] == 0)
      continue;
    // Read user input
    // Removing the newline from terminal input
    unsigned int len = strlen((char *)input) - 1;
    input[len]       = 0;
    // Decryption turns hex into their byte which is expected to be ascii
    if (decryptFlag) {
      unsigned char tmp[500];
      for (unsigned int i = 0; i < len; i++)
        sscanf((char *)&input[2 * i], "%2hhx", &tmp[i]);
      for (unsigned int i = 0; i < len / 2; i++)
        input[i] = tmp[i];
      bounce_decrypt(input, len / 2, key, keySum, output);
      for (unsigned int i = 0; i < len / 2; i++)
        printf("%c", output[i]);
    } else { // Encrypting
      bounce_encrypt(input, len, key, keySum, output);
      // Print the bytes as 2 ascii chars each, using hex
      for (unsigned int i = 0; i < len; i++)
        printf("%.2x", output[i]);
    }
    printf("\n");
  }
}

// Helper function to get key sum for first byte
unsigned char bounceProcKeySum(unsigned char *key) {
  unsigned char sum = 0;
  for (int i = 0; i < 256; i++)
    sum += key[i];
  return sum;
}
