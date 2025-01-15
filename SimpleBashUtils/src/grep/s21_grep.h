#ifndef __S21_GREP__
#define __S21_GREP__

#include <ctype.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "flags_process.h"

typedef struct {
  int findedCount;
  char **findedSubs;
} print_t;

bool openFile(char *fileName, FILE **file);
print_t find_substring(char *line, flags_t flags);
bool grepFileProcess(flags_t flags, FILE *file, const char *filename);
bool grep(flags_t flags);

#endif