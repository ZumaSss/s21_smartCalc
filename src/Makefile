CC = gcc
STDFLAGS = -Wall -Werror -Wextra -std=c11
CFLAGS_DEV = -g -Wall -Werror -Wextra -std=c11
LINUX_FLAGS = -lsubunit -lrt -lpthread -lm
LIBS = -lcheck
VALGRIND_FLAGS=--trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all --verbose

SRC = $(wildcard s21_*.c)
OBJ = $(patsubst %.c, %.o, ${SRC})

PREF_TEST_SRC = ./tests/
TEST_SRC = $(wildcard $(PREF_TEST_SRC)/s21_*.c)
GCOV_DIR = report

TARGET = s21_smart_calc.a

OS := $(shell uname -s)

ifeq ($(OS),Linux)
  OPEN_CMD = xdg-open
  TEST_FLAGS = $(LIBS) $(LINUX_FLAGS)
endif
ifeq ($(OS),Darwin)
  OPEN_CMD = open
  TEST_FLAGS = $(LIBS)
endif


all: $(TARGET)

$(TARGET): ${SRC}
	$(CC) -c $(STDFLAGS) $(SRC)
	ar rc $@ $(OBJ)
	ranlib $@

install:
	rm -rf smart_calc_qt/Build_Calc smart_calc_qt/QCustomPlot-library; \
	cd smart_calc_qt; \
	git clone https://github.com/legerch/QCustomPlot-library; \
	mkdir Build_Calc; \
	(cd Build_Calc && cmake ../); \
	(cd Build_Calc && make); \

dist: install 
	cd smart_calc_qt/Build_Calc
	tar --totals -cvf smart_calc.tar smart_calc_qt/Build_Calc/

dvi:
	doxygen Doxyfile
	open html/index.html

test: $(TARGET)
	${CC} $(STDFLAGS) ${TEST_SRC} $< -o unit_test $(TEST_FLAGS)
	./unit_test

gcov_report:
	make clean
	mkdir -p $(GCOV_DIR)
	$(CC) $(SRC) $(TEST_SRC) -o greport --coverage $(TEST_FLAGS)
	./greport
	lcov -t "s21_smart_calc" -o s21_smart_calc.info -c -d .
	genhtml -o ./$(GCOV_DIR) s21_smart_calc.info
	$(OPEN_CMD) ./report/index.html


valgrind: $(TARGET)
	$(CC) $(CFLAGS_DEV) ${TEST_SRC} $< -o test $(TEST_FLAGS)
	CK_FORK=no valgrind $(VALGRIND_FLAGS) --log-file=RESULT_VALGRIND.txt ./test

dev:
	$(CC) $(CFLAGS_DEV) $(SRC) $(LINUX_FLAGS)
	./a.out

style: clean
	cp ../materials/linters/.clang-format ../src/.clang-format
	clang-format -n *.c *.h $(PREF_TEST_SRC)*.c $(PREF_TEST_SRC)*.h smart_calc_qt/*.h smart_calc_qt/*.cpp
	rm .clang-format

uninstall:
	rm -rf smart_calc_qt/Build_Calc
	rm -rf smart_calc_qt/QCustomPlot-library
	rm -rf ./smart_calc_qt/build*

clean:
	rm -rf unit_test test greport html latex
	rm -rf RESULT_VALGRIND.txt
	rm -rf *.o *.a *.tar
	rm -rf *.gcda *.gcno *.info
	rm -rf s21_test report
	rm -rf s21_calc s21_test