#ifndef _S21_CAT_
#define _S21_CAT_

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  bool show_not_empty_numbers;
  bool show_numbers;
  bool show_ends;
  bool show_tabs;
  bool show_nonprinting;
  bool squeeze_blank;
} flags_t;

void s21_cat(int countFlags, flags_t flags, char **argv, int argc);
bool fillFlags(flags_t *flags, char *flag);
flags_t process_flags(int argc, char *argv[], int *countFlags);
void print_file(FILE *file, flags_t flags);

#endif