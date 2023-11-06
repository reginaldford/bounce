// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include "bounce.h"

// Returns a bounceReadFileResult
bounceReadFileResult bounceReadFile(const char *filePath) {
  size_t fileSize = 0;
  FILE  *file     = fopen(filePath, "rb");
  if (file == NULL) {
    perror("Error opening file");
    bounceReadFileResult result = {
      .fileExists  = 0,
      .fileSize    = 0,
      .fileContent = NULL,
    };
    return result;
  }

  // Determine the file size
  fseek(file, 0, SEEK_END);
  fileSize = ftell(file);
  fseek(file, 0, SEEK_SET);

  // Allocate memory for the file contents
  unsigned char *fileData = (unsigned char *)malloc(fileSize);
  if (fileData == NULL) {
    perror("Error allocating memory");
    fclose(file);
    bounceReadFileResult result = {
      .fileExists  = 0,
      .fileSize    = 0,
      .fileContent = NULL,
    };
    return result;
  }

  // Read the file contents into the allocated memory
  size_t bytesRead = fread(fileData, 1, fileSize, file);
  if (bytesRead != fileSize) {
    perror("Error reading file");
    free(fileData);
    fclose(file);
    bounceReadFileResult result = {
      .fileExists  = 0,
      .fileSize    = 0,
      .fileContent = NULL,
    };
    return result;
  }

  //
  fclose(file);
  bounceReadFileResult result = {
    .fileExists  = 1,
    .fileSize    = fileSize,
    .fileContent = fileData,
  };
  return result;
}
