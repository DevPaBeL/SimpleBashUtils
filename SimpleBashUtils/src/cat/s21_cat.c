#include "s21_cat.h"

int main(int argc, char *argv[]) {
  int countFlags = 1;
  flags_t flags = process_flags(argc, argv, &countFlags);
  if (argc < 2 || countFlags == argc) {
    print_file(stdin, flags);
  } else {
    s21_cat(countFlags, flags, argv, argc);
  }
  return 0;
}

void s21_cat(int countFlags, flags_t flags, char **argv, int argc) {
  int i;
  FILE *file;
  for (i = countFlags; i < argc; i++) {
    file = fopen(argv[i], "r");
    if (file == NULL) {
      fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
      continue;
    }
    print_file(file, flags);
    fclose(file);
  }
}

bool fillFlags(flags_t *flags, char *flag) {
  bool isTrueFlag = true;
  if (strcmp(flag, "-b") == 0 || strcmp(flag, "--number-nonblank") == 0) {
    flags->show_not_empty_numbers = true;
  } else if (strcmp(flag, "-e") == 0 || strcmp(flag, "-E") == 0) {
    if (strcmp(flag, "-e") == 0) flags->show_nonprinting = true;
    flags->show_ends = true;
  } else if (strcmp(flag, "-t") == 0 || strcmp(flag, "-T") == 0) {
    if (strcmp(flag, "-t") == 0) flags->show_nonprinting = true;
    flags->show_tabs = true;
  } else if (strcmp(flag, "-v") == 0 ||
             strcmp(flag, "--show-nonprinting") == 0) {
    flags->show_nonprinting = true;
  } else if (strcmp(flag, "-s") == 0 || strcmp(flag, "--squeeze-blank") == 0) {
    flags->squeeze_blank = true;
  } else if (strcmp(flag, "-n") == 0 || strcmp(flag, "--number") == 0) {
    flags->show_numbers = true;
  } else {
    isTrueFlag = false;
  }
  return isTrueFlag;
}

flags_t process_flags(int argc, char *argv[], int *countFlags) {
  flags_t flags = {false};
  int i;
  for (i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && strlen(argv[i]) > 2 && argv[i][1] != '-') {
      for (size_t j = 1; j < strlen(argv[i]); j++) {
        char flag[3] = {'-', argv[i][j], '\0'};
        fillFlags(&flags, flag);
      }
      *countFlags += 1;
    } else {
      *countFlags += fillFlags(&flags, argv[i]);
    }
  }
  return flags;
}

void print_file(FILE *file, flags_t flags) {
  char current = fgetc(file);
  char previosChar = '\n';
  bool lineWasEmpty = false;
  bool lineIsEmpty = false;
  int lineNumber = 1;
  while (current != EOF) {
    lineIsEmpty = (previosChar == '\n' && current == '\n');
    if (flags.squeeze_blank && lineWasEmpty && lineIsEmpty) {
      previosChar = current;
      current = fgetc(file);
      continue;
    }
    if (flags.show_numbers && !flags.show_not_empty_numbers &&
        previosChar == '\n') {
      printf("%6d	", lineNumber);
      lineNumber++;
    } else if (flags.show_not_empty_numbers && previosChar == '\n' &&
               !lineIsEmpty) {
      printf("%6d	", lineNumber);
      lineNumber++;
    }
    if (flags.show_tabs && current == '\t') {
      printf("^I");
    } else if (flags.show_ends && current == '\n') {
      if (flags.show_not_empty_numbers && lineIsEmpty) printf("      \t");
      printf("$\n");
    } else if (flags.show_nonprinting && !isprint(current) && current != '\t' &&
               current != '\n') {
      if (current != 127)
        printf("^%c", current + 64);
      else
        printf("^%c", current - 64);
    } else {
      putchar(current);
    }
    previosChar = current;
    lineWasEmpty = lineIsEmpty;
    current = fgetc(file);
  }
}