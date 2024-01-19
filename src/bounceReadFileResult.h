// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

typedef struct bounceReadFileResult {
  int      fileExists;
  int      fileSize;
  uint8_t *fileContent;
} bounceReadFileResult;
