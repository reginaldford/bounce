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

// Structure for results of bounceReadFile function
#include "bounceReadFileResult.h"

#define SQ(x) (x * x)

// bounceEnc.c
unsigned char *bounce_encrypt(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                              unsigned int keySum1, unsigned int keySum2, unsigned char *table,
                              unsigned char *output);
unsigned char *bounce_encrypt_pass(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                                   unsigned char *table, unsigned char *output);
// bounceDec.c
unsigned char *bounce_decrypt(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                              unsigned int keySum1, unsigned int keySum2, unsigned char *table,
                              unsigned char *output);
unsigned char *bounce_decrypt_pass(unsigned char *msg, unsigned int msgLen, unsigned char *key,
                                   unsigned char *table, unsigned char *output);
// bounceReadFile.c
bounceReadFileResult bounceReadFile(const char *filePath);

// bounceGenKey.c
void bounceGenKey(FILE *fp);

// bounceProc.c
void         bounceProcess(FILE *inFile, FILE *outfile, unsigned char *key, bool decryptFlag);
unsigned int bounceProcKeySum(unsigned char *key);
void         bounceProcSubTable(unsigned char *key, unsigned char *table);
void         bounceREPL(unsigned char *key, int decryptFlag);
// bounceRoll.c
void bounce_roll(unsigned char *msg, unsigned int msgLen, unsigned char *output,
                 unsigned int keySum1, unsigned int keySum2);
void bounce_roll_lr(unsigned char *msg, unsigned int msgLen, unsigned char *output,
                    unsigned int keySum);
void bounce_roll_rl(unsigned char *msg, unsigned int msgLen, unsigned char *output,
                    unsigned int keySum);
void bounce_unroll(unsigned char *msg, unsigned int msgLen, unsigned char *output,
                   unsigned int keySum1, unsigned int keySum2);
void bounce_unroll_lr(unsigned char *msg, unsigned int msgLen, unsigned char *output,
                      unsigned int keySum);
void bounce_unroll_rl(unsigned char *msg, unsigned int msgLen, unsigned char *output,
                      unsigned int keySum);
