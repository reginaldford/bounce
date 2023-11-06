// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

void bounceGiveHelp(char **args) {
  printf("Bounce v0.01\n");
  printf("Copyright (c) 2023 Rex Ford\n\n");
  printf("Uses and examples:\n");
  printf("1. Generate a key:\n");
  printf("  %s -g > myKey.k\n\n", args[0]);
  printf("2. Encrypt a file:\n");
  printf("  %s -k myKey.k -i msg.txt -o msg.txt.b\n\n", args[0]);
  printf("3. Decrypt a file (note the -d):\n");
  printf("  %s -k myKey.k -i msg.txt.b -o msg.txt -d\n\n", args[0]);
  printf("You may also skip -o flag and pipe the output to a file.\n");
  printf("  %s -k myKey.k -i msg.txt.b -d > clear.txt\n", args[0]);
  fflush(stdout);
  exit(1);
}

int main(int num_args, char **args) {
  int   opt;
  char *inFilePath      = NULL; // i input file
  char *keyFilePath     = NULL; // k key
  char *outFilePath     = NULL; // o output file (stdout is default)
  bool  decryptFlag     = 0;    // d decrypt (enc is default)
  bool  genKeyFlag      = 0;    // g generate key
  bool  optionsWereUsed = 0;
  while ((opt = getopt(num_args, args, "k:di:o:gh")) != -1) {
    optionsWereUsed = 1;
    switch (opt) {
    case 'k':
      keyFilePath = optarg;
      break;
    case 'd':
      decryptFlag = 1;
      break;
    case 'i':
      inFilePath = optarg;
      break;
    case 'o':
      outFilePath = optarg;
      break;
    case 'g':
      genKeyFlag = 1;
      break;
    case 'h':
      bounceGiveHelp(args);
      return 1;
      break;
    }
  }

  // If no flags were used, let's give help info
  if (optionsWereUsed == 0) {
    bounceGiveHelp(args);
    return 1;
  }

  // Gen Key is special case
  if (genKeyFlag) {
    bounceGenKey(stdout);
    return 0;
  }

  // Read key file
  if (keyFilePath == 0) {
    printf("No key file provided. Use -k\n");
    exit(EXIT_FAILURE);
  }
  bounceReadFileResult keyFileResult = bounceReadFile(keyFilePath);
  if (keyFileResult.fileExists == 0) {
    printf("Could not read key file\n"); // should use stderr
    exit(EXIT_FAILURE);
  }
  unsigned char *key = keyFileResult.fileContent;

  // Read input file
  if (inFilePath == 0) {
    printf("No input file provided. Use -i\n");
    exit(EXIT_FAILURE);
  }
  bounceReadFileResult inFileResult = bounceReadFile(inFilePath);
  if (inFileResult.fileExists == 0) {
    printf("Could not read input file\n"); // should use stderr
    exit(EXIT_FAILURE);
  }
  unsigned char *input = inFileResult.fileContent;

  // Setup the output buffer
  int           inputLen = inFileResult.fileSize;
  unsigned char output[inputLen];

  // Process the data
  if (decryptFlag)
    bounce_decrypt(input, inputLen, key, output);
  else
    bounce_encrypt(input, inputLen, key, output);

  // Perhaps Output to file
  if (outFilePath) {
    FILE *outputFile = fopen(outFilePath, "wb");

    if (outputFile == NULL) {
      perror("fopen");
      // Handle fopen error
      return 1;
    }

    // Writing data to file
    for (int i = 0; i < inputLen; i++)
      putc(output[i], outputFile);

    if (fclose(outputFile) != 0) {
      perror("fclose");
      return 1;
    }
    free(input);
    return 0;
  }

  // Or Output to stdout
  for (int i = 0; i < inputLen; i++)
    putc(output[i], stdout);
  free(input);
  return 0;
}
