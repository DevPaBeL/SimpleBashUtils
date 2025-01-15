#ifndef __FLAGS_PROCESS__
#define __FLAGS_PROCESS__

#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_MATCHES 10

typedef struct {
  bool e;
  bool i;
  bool c;
  bool v;
  bool l;
  bool n;
  bool h;
  bool o;
  bool s;
  bool f;
  char **substrings;
  char **files;
  int substringCount;
  int filesCount;
  int reti;
  regex_t regex;
  regmatch_t matches[MAX_MATCHES];
} flags_t;

bool fillStringArray(char ***array, char *string, int *stringsCount, int start,
                     int end);
bool fillFlags(flags_t *flags, char *flag, bool *isEFlag, bool *isFFlag);
flags_t process_flags(int argc, char *argv[], bool *success);
void freeArray(char **array, int count);
char *read_line(FILE *file);

#endif