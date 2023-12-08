// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

void bounceGiveHelp(char **args) {
  printf("Bounce v0.03\n");
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
  printf("Bounce accepts data pipes for input and/or output:\n");
  printf("echo \"secret\" | bounce -k k | bounce -k k -d\n");
  fflush(stdout);
  exit(1);
}

int main(int num_args, char **args) {
  int   opt;
  char *inFilePath      = NULL;   // i Input file
  char *keyFilePath     = NULL;   // k Key
  char *outFilePath     = NULL;   // o Output file path (stdout is default)
  FILE *inFile          = stdin;  // Input file ptr
  FILE *outFile         = stdout; // Output file ptr
  bool  decryptFlag     = false;  // D decrypt (enc is default)
  bool  genKeyFlag      = false;  // G generate key
  bool  optionsWereUsed = false;  // Whether any options where used
  while ((opt = getopt(num_args, args, "k:di:o:ghf")) != -1) {
    optionsWereUsed = true;
    switch (opt) {
    case 'k':
      keyFilePath = optarg;
      break;
    case 'd':
      decryptFlag = true;
      break;
    case 'i':
      inFilePath = optarg;
      break;
    case 'o':
      outFilePath = optarg;
      break;
    case 'g':
      genKeyFlag = true;
      break;
    case 'h':
      bounceGiveHelp(args);
      return true;
      break;
    }
  }

  // If no flags were used, let's give help info
  if (optionsWereUsed == 0) {
    bounceGiveHelp(args);
    return 1;
  }

  // Perhaps Output to file
  if (outFilePath) {
    outFile = fopen(outFilePath, "wb");
    // Handle fopen error
    if (outFile == NULL) {
      perror("fopen");
      return 1;
    }
  }

  // Gen Key is special case
  if (genKeyFlag) {
    bounceGenKey(outFile);
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

  // If -i was used, open input file
  if (inFilePath) {
    inFile = fopen(inFilePath, "rb");
    // Handle fopen error
    if (inFile == NULL) {
      perror("fopen");
      return 1;
    }
  }

  // Processing
  bounceProcess(inFile, outFile, key, decryptFlag);

  // Close output file , if we opened one
  if (outFile != stdout && fclose(outFile) != 0)
    perror("fclose");

  // Clean up and return 0
  return 0;
}
