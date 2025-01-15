#include "s21_grep.h"
void error(char **argv) {
  fprintf(stderr, "Usage: %s <substring> <filename1> [<filename2> ...]\n",
          argv[0]);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    error(argv);
  }

  bool success = true;
  flags_t flags = process_flags(argc, argv, &success);
  if (success) {
    success = grep(flags);
    freeArray(flags.files, flags.filesCount);
    freeArray(flags.substrings, flags.substringCount);
  }
  return !success;
}

bool openFile(char *fileName, FILE **file) {
  bool success = true;
  *file = fopen(fileName, "r");
  if (*file == NULL) success = false;
  return success;
}

print_t find_substring(char *line, flags_t flags) {
  char *newLine = line;
  print_t print = {0, NULL};
  bool findedInLine = false;
  for (int i = 0; i < flags.substringCount; i++) {
    char *newSub = flags.substrings[i];
    if (!flags.i)
      flags.reti = regcomp(&flags.regex, newSub, REG_EXTENDED);
    else
      flags.reti = regcomp(&flags.regex, newSub, REG_ICASE);
    if (!flags.reti) {
      char *newLine = line;
      if (flags.v) {
        flags.reti =
            regexec(&flags.regex, newLine, MAX_MATCHES, flags.matches, 0);
        if (flags.reti && !findedInLine) {
          fillStringArray(&print.findedSubs, newLine, &print.findedCount, 0, 0);
          print.findedCount = 1;
        } else {
          if (print.findedCount) {
            free(print.findedSubs[print.findedCount]);
            print.findedCount--;
          }
          findedInLine = true;
        }
      } else {
        while ((flags.reti = regexec(&flags.regex, newLine, MAX_MATCHES,
                                     flags.matches, 0)) == 0) {
          if (!flags.reti) {
            int tmp = 0;
            if (flags.substrings[i][0] == '^') tmp = 1;
            fillStringArray(&print.findedSubs, flags.substrings[i],
                            &print.findedCount, tmp, 0);
            newLine += flags.matches[0].rm_eo;
          }
        }
      }
      regfree(&flags.regex);
    } else if (!flags.s) {
      printf("grep: cannot compile pattern with: %s\n", newLine);
    }
  }
  return print;
}

bool grep(flags_t flags) {
  bool success = true;
  for (int i = 0; i < flags.filesCount; i++) {
    FILE *file = NULL;
    success = openFile(flags.files[i], &file);
    if (success) {
      success = grepFileProcess(flags, file, flags.files[i]);
    } else if (!flags.s) {
      printf("grep: %s: No such file or directory\n", flags.files[i]);
    }
  }
  return success;
}

void printFindedLine(flags_t flags, print_t print, const char *filename,
                     char *line, int numberLine) {
  if (!flags.o) {
    if (!flags.h) printf("%s:", filename);
    if (flags.n) printf("%d:", numberLine);
    printf("%s\n", line);
  } else {
    for (int i = 0; i < print.findedCount; i++) {
      if (!flags.h) printf("%s:", filename);
      if (flags.n) printf("%d:", numberLine);
      printf("%s\n", print.findedSubs[i]);
    }
  }
}

bool grepFileProcess(flags_t flags, FILE *file, const char *filename) {
  char *line = NULL;
  int findedInFileCount = 0;
  int numberLineInFile = 0;

  while (!feof(file)) {
    line = read_line(file);
    if (line != NULL) {
      numberLineInFile++;
      print_t print = find_substring(line, flags);
      if (print.findedCount) {
        findedInFileCount++;
        if (!flags.c && !flags.l) {
          printFindedLine(flags, print, filename, line, numberLineInFile);
        }
        freeArray(print.findedSubs, print.findedCount);
      }
      free(line);
    }
  }
  if (flags.c && !flags.l) {
    if (!flags.h) printf("%s:", filename);
    printf("%d\n", findedInFileCount);
  }
  if (flags.l) {
    if (!flags.h && flags.c) printf("%s:", filename);
    if (flags.c) printf("%d\n", (bool)findedInFileCount);
    if (findedInFileCount) printf("%s\n", filename);
  }
  return findedInFileCount;
}