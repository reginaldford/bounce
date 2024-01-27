// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
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
uint8_t *bounce_encrypt(uint8_t *msg, uint32_t msgLen, uint8_t *key, uint32_t keySum1,
                        uint32_t keySum2, uint8_t *table, uint8_t *output);
uint8_t *bounce_encrypt_pass(uint8_t *msg, uint32_t msgLen, uint8_t *key, uint8_t *table,
                             uint8_t *output);
uint8_t  bounce_trade_byte(uint8_t byte);
uint8_t *bounce_decrypt(uint8_t *msg, uint32_t msgLen, uint8_t *key, uint32_t keySum1,
                        uint32_t keySum2, uint8_t *table, uint8_t *output);
uint8_t *bounce_decrypt_pass(uint8_t *msg, uint32_t msgLen, uint8_t *key, uint8_t *table,
                             uint8_t *output);
// bounceReadFile.c
bounceReadFileResult bounceReadFile(const char *filePath);

// bounceGenKey.c
void bounceGenKey(FILE *fp);

// bounceProc.c
void     bounceProcess(FILE *inFile, FILE *outfile, uint8_t *key, bool decryptFlag);
uint32_t bounceProcKeySum(uint8_t *key);
void     bounceREPL(uint8_t *key, int decryptFlag);

// bounceRoll.c
void bounce_roll(uint8_t *msg, uint32_t msgLen, uint8_t *output, uint32_t keySum1, uint32_t keySum2,
                 uint8_t *table);
void bounce_roll_lr(uint8_t *msg, uint32_t msgLen, uint8_t *output, uint32_t keySum,
                    uint8_t *table);
void bounce_roll_rl(uint8_t *msg, uint32_t msgLen, uint8_t *output, uint32_t keySum,
                    uint8_t *table);
void bounce_unroll(uint8_t *msg, uint32_t msgLen, uint8_t *output, uint32_t keySum1,
                   uint32_t keySum2, uint8_t *table);
void bounce_unroll_lr(uint8_t *msg, uint32_t msgLen, uint8_t *output, uint32_t keySum,
                      uint8_t *table);
void bounce_unroll_rl(uint8_t *msg, uint32_t msgLen, uint8_t *output, uint32_t keySum,
                      uint8_t *table);
// bounceTable.c
void bounceProcSubTable(uint8_t *key, uint8_t *table);
