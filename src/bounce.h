// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include "bounceReadFileResult.h"

unsigned char *bounce_encrypt(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                              unsigned char *output);
unsigned char *bounce_encrypt_pass_lr(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                                      unsigned char *output);
unsigned char *bounce_encrypt_pass_rl(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                                      unsigned char *output);
unsigned char *bounce_decrypt(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                              unsigned char *output);
unsigned char *bounce_decrypt_pass_lr(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                                      unsigned char *output);
unsigned char *bounce_decrypt_pass_rl(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                                      unsigned char *output);
bounceReadFileResult bounceReadFile(const char *filePath);
void                 bounceGenKey(FILE *fp);

void bounceProcess(FILE *inFile, FILE *outfile, unsigned char *key, bool decryptFlag);

unsigned char *bounceGenAlt(unsigned char *key);
