// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

void bounceGiveHelp(char **args) {
  char       *yellow    = "";
  char       *green     = "";
  char       *blue      = "";
  char       *clear     = "";
  const char *term      = getenv("TERM");
  const char *colorterm = getenv("COLORTERM");

  // Detect if we should use color
  if (term != NULL && (colorterm != NULL || strstr(term, "color") != NULL)) {
    yellow = "\033[33m";
    green  = "\033[32m";
    blue   = "\033[34m";
    clear  = "\033[0m";
  }

  printf("%s", green);
  printf("Bounce v0.051\n");
  printf("%s", blue);
  printf("Copyright (c) 2023 Rex Ford\n");
  printf("%s", clear);
  printf("Source code and BSD license is at github.com/reginaldford/bounce\n\n");
  printf("Uses and examples:\n");
  printf("1. Generate a key:\n");
  printf("%s", yellow);
  printf("  %s -g > myKey\n\n", args[0]);
  printf("%s", clear);
  printf("2. Encrypt a file:\n");
  printf("%s", yellow);
  printf("  %s -k myKey -i msg.txt -o msg.txt.b\n\n", args[0]);
  printf("%s", clear);
  printf("3. Decrypt a file (note the -d):\n");
  printf("%s", yellow);
  printf("  %s -k myKey -i msg.txt.b -o msg.txt -d\n\n", args[0]);
  printf("%s", clear);
  printf("Note: Bounce accepts data pipes for input and/or output.\n");
  printf("Omitting -i and/or -o uses a pipe instead of a file:\n");
  printf("%s", yellow);
  printf("  echo \"secret\" | bounce -k k | bounce -k k -d\n");
  printf("%s", clear);
  printf("4. REPL allows conversion to and from clear text and encrypted hex.\n");
  printf("%s", yellow);
  printf("  %s -k myKey -r\n", args[0]);
  printf("%s", clear);
  printf("Adding -d to above command will create a decrypting REPL.\n");
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
  bool  replFlag        = false;  // REPL mode flag
  bool  optionsWereUsed = false;  // Whether any options where used
  while ((opt = getopt(num_args, args, "k:di:o:ghfr")) != -1) {
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
    case 'r':
      replFlag = true;
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

  // REPL Mode
  if (replFlag) {
    printf("REPL mode. ");
    if (decryptFlag)
      printf(" (Decrypting) ");
    else
      printf(" (Encrypting) ");
    printf("Max message size is 500 characters. Ctrl+C to end.\n\n");
    bounceREPL(key, decryptFlag);
  } else { // Regular processing
    bounceProcess(inFile, outFile, key, decryptFlag);
  }

  // Close output file , if we opened one
  if (outFile != stdout && fclose(outFile) != 0)
    perror("fclose");

  // Clean up and return 0
  return 0;
}
