CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11

s21_grep: s21_grep.c flags_process.c
	$(CC) -c $(CFLAGS) s21_grep.c flags_process.c
	$(CC) $(CFLAGS) *.o -o s21_grep
clean:
	rm -rf *.o
	rm -rf s21_grep