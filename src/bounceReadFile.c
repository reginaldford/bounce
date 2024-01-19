// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

// Returns a bounceReadFileResult
bounceReadFileResult bounceReadFile(const char *filePath) {
  size_t fileSize = 0;
  FILE  *file     = fopen(filePath, "rb");
  // Generic result
  bounceReadFileResult result = {
    .fileExists  = 0,
    .fileSize    = 0,
    .fileContent = NULL,
  };
  if (file == NULL) {
    perror("Error opening file");
    return result;
  }
  // Determine the file size
  fseek(file, 0, SEEK_END);
  fileSize = ftell(file);
  fseek(file, 0, SEEK_SET);
  // Allocate memory for the file contents
  uint8_t *fileData = (uint8_t *)malloc(fileSize);
  if (fileData == NULL) {
    perror("Error allocating memory");
    fclose(file);
    return result;
  }
  // Read the file contents into the allocated memory
  size_t bytesRead = fread(fileData, 1, fileSize, file);
  if (bytesRead != fileSize) {
    perror("Error reading file");
    free(fileData);
    fclose(file);
    return result;
  }
  // Close file and return good results
  fclose(file);
  result.fileExists  = 1;
  result.fileSize    = fileSize;
  result.fileContent = fileData;
  return result;
}
