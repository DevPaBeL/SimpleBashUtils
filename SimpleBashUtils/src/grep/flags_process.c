#include "flags_process.h"

bool fillStringArray(char ***array, char *string, int *stringsCount, int start,
                     int end) {
  bool success = true;
  char **newArray =
      (char **)realloc(*array, sizeof(char *) * (*stringsCount + 1));
  if (newArray == NULL) {
    success = false;
  } else {
    newArray[*stringsCount] = (char *)realloc(
        NULL, sizeof(char) * (strlen(string + start) + 1 - end));
    if (newArray[*stringsCount] == NULL) {
      success = false;
    } else {
      strncpy(newArray[*stringsCount], string + start,
              strlen(string + start) + 1 - end);
      *stringsCount += 1;
    }
  }

  if (!success) {
    freeArray(newArray, *stringsCount);
  } else {
    *array = newArray;
  }

  return success;
}

void checkForExamples(flags_t *flags) {
  int j = flags->substringCount;
  if (!flags->e && j != 0 && !flags->f) {
    for (int i = 0; i < flags->substringCount; i++) {
      fillStringArray(&flags->files, flags->substrings[i], &flags->filesCount,
                      0, 0);
      free(flags->substrings[i]);
      flags->substringCount--;
    }
  }
}

bool fillFlags(flags_t *flags, char *flag, bool *isEFlag, bool *isFFlag) {
  bool isTrueFlag = true;
  if (strcmp(flag, "-e") == 0) {
    checkForExamples(flags);
    *isEFlag = true;
    flags->e = true;
  } else if (strcmp(flag, "-i") == 0) {
    flags->i = true;
  } else if (strcmp(flag, "-v") == 0) {
    flags->v = true;
  } else if (strcmp(flag, "-c") == 0) {
    flags->c = true;
  } else if (strcmp(flag, "-l") == 0) {
    flags->l = true;
  } else if (strcmp(flag, "-n") == 0) {
    flags->n = true;
  } else if (strcmp(flag, "-h") == 0) {
    flags->h = true;
  } else if (strcmp(flag, "-o") == 0) {
    flags->o = true;
  } else if (strcmp(flag, "-s") == 0) {
    flags->s = true;
  } else if (strcmp(flag, "-f") == 0) {
    flags->f = true;
    *isFFlag = true;
  } else {
    isTrueFlag = false;
  }
  return isTrueFlag;
}

char *read_line(FILE *file) {
  int capacity = 64;
  int length = 0;
  char *line = (char *)malloc(capacity * sizeof(char));

  if (!line) {
    line = NULL;
  } else {
    int c;
    while ((c = fgetc(file)) != EOF && c != '\n' && line != NULL) {
      line[length++] = (char)c;
      if (length >= capacity) {
        capacity *= 2;
        char *new_line = (char *)realloc(line, capacity * sizeof(char));
        if (!new_line) {
          free(line);
          line = NULL;
        }
        line = new_line;
      }
    }
    line[length] = '\0';
    if (length == 0 && c == EOF) {
      free(line);
      line = NULL;
    }
  }

  return line;
}

bool fillFromFile(char *fileName, flags_t *flags) {
  bool success = true;
  bool isEmpty = true;
  FILE *file = fopen(fileName, "r");
  if (file == NULL)
    success = false;
  else {
    while (!feof(file)) {
      char *line = read_line(file);
      if (line != NULL) {
        isEmpty = false;
        fillStringArray(&flags->substrings, line, &flags->substringCount, 0, 0);
        free(line);
      }
    }
    fclose(file);
  }
  success = !isEmpty;
  return success;
}

flags_t process_flags(int argc, char *argv[], bool *success) {
  flags_t flags = {};
  for (int i = 1; i < argc && *success; i++) {
    if (argv[i][0] == '-' && strlen(argv[i]) > 2) {
      bool isEFlag = false;
      bool isFFlag = false;
      for (int j = 1; j < (int)strlen(argv[i]) && !isEFlag && !isFFlag; j++) {
        char flag[3] = {'-', argv[i][j], '\0'};
        fillFlags(&flags, flag, &isEFlag, &isFFlag);
        if (isEFlag) {
          *success = fillStringArray(&flags.substrings, argv[i],
                                     &flags.substringCount, 2, 0);
        } else if (isFFlag) {
          char **tmp = NULL;
          int null = 0;
          *success = fillStringArray(&tmp, argv[i], &null, 2, 0);
          if (success) {
            *success = fillFromFile(tmp[0], &flags);
            freeArray(tmp, 1);
          }
        }
      }
    } else if (argv[i][0] == '-') {
      bool isEFlag = false;
      bool isFFlag = false;
      fillFlags(&flags, argv[i], &isEFlag, &isFFlag);
      if (isEFlag && argv[i + 1]) {
        *success = fillStringArray(&flags.substrings, argv[i + 1],
                                   &flags.substringCount, 0, 0);
        i++;
      } else if (isFFlag && argv[i + 1]) {
        *success = fillFromFile(argv[i + 1], &flags);
        i++;
      } else if (isEFlag || isFFlag) {
        *success = false;
      }
    } else {
      if (flags.substringCount == 0 && !flags.e) {
        *success = fillStringArray(&flags.substrings, argv[i],
                                   &flags.substringCount, 0, 0);
      } else {
        *success =
            fillStringArray(&flags.files, argv[i], &flags.filesCount, 0, 0);
      }
    }
  }
  flags.h = (flags.h || flags.filesCount == 1);
  return flags;
}

void freeArray(char **array, int count) {
  for (int i = 0; i < count; i++) {
    free(array[i]);
  }
  free(array);
}