CFLAGS = -c -Wall -Werror -Wextra -std=c11 -pedantic -lm
TST_FLAGS = $(shell pkg-config --cflags check)
TST_LIBS = $(shell pkg-config --libs check)
SOURCES = 3d_process.c test.c
OBJECTS = *.o
OS = $(shell uname -s)
BUILD = build


all: tests

tests:
	gcc $(CFLAGS) $(SOURCES) $(TST_FLAGS)
	gcc $(OBJECTS) $(TST_LIBS)
	-./a.out

gcov_report:
	gcc -fprofile-arcs -ftest-coverage $(SOURCES) $(TST_LIBS)
	-./a.out
	lcov --no-external -t "test" -o test.info -c -d .
	genhtml -o report test.info
ifeq ($(OS), Darwin)
	-open report/index.html
else
	-xdg-open report/index.html
endif

install:
	make -C ./$(BUILD)

uninstall:
	make clean -C ./$(BUILD)
	rm -rf ./$(BUILD)/3Dviewer.app

dist:
	tar -czf 3DViewer.tar.gz .

dvi:
ifeq ($(OS), Darwin)
	open dvi.html
else
	xdg-open dvi.html
endif

leaks:
	gcc $(CFLAGS) $(SOURCES) $(TST_FLAGS)
	gcc $(OBJECTS) $(TST_LIBS)
	-./a.out
ifeq ($(OS), Darwin)
	CK_FORK=no leaks --atExit -- ./a.out
else
	valgrind --leak-check=full --quiet ./a.out
endif

check:
	cp ../materials/linters/CPPLINT.cfg ./
	-python3 ../materials/linters/cpplint.py --extensions=c $(SOURCES)
	rm *.cfg
	cppcheck --enable=all --suppress=missingIncludeSystem ./*.c

.PHONY: all clean rebuild install uninstall

clean:
	rm -rf *.o *.so *.gch *.gcda *.gcno *.info *.out
	rm -rf report

rebuild: clean all
