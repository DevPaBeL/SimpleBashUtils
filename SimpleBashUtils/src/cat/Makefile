FLAGS = -Wall -Wextra -Werror
all: clean s21_cat

s21_cat: s21_cat.h s21_cat.c
	gcc $(FLAGS) s21_cat.c -o s21_cat

clang:
	clang-format -n -style google *.c *.h
	clang-format -i -style google *.c *.h

rebuild:
	make all

clean:
	rm s21_cat
test:
	make all
	sh test.bash