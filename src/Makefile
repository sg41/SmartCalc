LDOPTS = -L. -lm -lcheck
CCOPTS = -Wall -Werror -Wextra -DNDEBUG -g
GTK_OPTS = -g -rdynamic `pkg-config --cflags gtk+-3.0`
GTK_LIBS = `pkg-config --libs gtk+-3.0`
CC = gcc
APP_ARGS = "sin(cos(x^2)^(1*-100))*x"
TEST_APP = test.app
TEST_C = tests/test.c
DOCKER_NAME=calc:1.01
RELEASE_NAME=s21_SmartCalc-1.01
TEXI2DVI=texi2dvi
.SUFFIXES:

# ROOT := $(shell git rev-parse --show-toplevel)
ROOT := ../
GCOV=-fprofile-arcs -ftest-coverage
OS=$(shell uname)
ifeq ($(OS), Linux)
	LIBS=-lcheck -lpthread -lrt -lm -lsubunit
	LEAKS=CK_FORK=no valgrind -s --tool=memcheck --leak-check=full --show-leak-kinds=all --show-reachable=yes 
else
	LIBS=-lcheck
	LEAKS=CK_FORK=no leaks --atExit -- 
endif

APP = graph.app
LIB = s21_SmartCalc
SRC_DIR=$(ROOT)/src
OBJECTS = rpr/llist.o rpr/stack.o rpr/expr.o rpr/core.o
SOURCES = $(OBJECTS:.o=.c)
HEADERS = $(OBJECTS:.o=.h)
INST_DIR=../build

all:  graph gtk 

dvi: doc.dvi

doc.dvi: doc.texi
	$(TEXI2DVI) $(SRC_DIR)/doc.texi

dist: clean $(SOURCES) graph.c graph.h gtk.c gtk.h
	[ -d $(RELEASE_NAME) ] || mkdir $(RELEASE_NAME)
	cp *.c *.h Makefile Dockerfile newversion-v9.ui run.sh ../LICENSE ../README.md $(RELEASE_NAME)/
	mkdir $(RELEASE_NAME)/rpr
	cp rpr/*.c $(RELEASE_NAME)/rpr/
	cp rpr/*.h $(RELEASE_NAME)/rpr/
	tar -czf $(RELEASE_NAME).tar.gz $(RELEASE_NAME)
	rm -rf $(RELEASE_NAME)
	
install: $(INST_DIR)/gtk.app

$(INST_DIR)/gtk.app: gtk $(APP)
	[ -d $(INST_DIR) ] || mkdir $(INST_DIR)
	cp gtk.app newversion-v9.ui $(APP) $(INST_DIR)
	cp run.sh $(INST_DIR)

uninstall: 
	rm -rf $(INST_DIR)/gtk.app $(INST_DIR)/newversion-v9.ui $(INST_DIR)/$(APP) $(INST_DIR)/run.sh
	-[ `ls -a $(INST_DIR)| wc -l` -eq 2 ] && rm -rf $(INST_DIR)

$(LIB): lib$(LIB).a

lib$(LIB).a: $(OBJECTS)
	ar rcs lib$(LIB).a $(OBJECTS)

gtk: gtk.app

gtk.app: gtk.c	$(LIB)
	$(CC) $(GTK_OPTS) gtk.c -o gtk.app $(LDOPTS) -l$(LIB) $(LIBS) $(GTK_LIBS)

graph: $(LIB) graph.c graph.h rpr/llist.h rpr/stack.h
	gcc $(CCOPTS) -DTXT_VERSION graph.c -o $(APP) -l$(LIB) $(LDOPTS)

$(APP): graph

graph.o: graph.c graph.h rpr/llist.h
	gcc $(CCOPTS) $(LDOPTS) -c graph.c

rpr/llist.o: rpr/llist.c rpr/llist.h graph.h
	gcc $(CCOPTS) $(LDOPTS) -c rpr/llist.c -o rpr/llist.o

rpr/stack.o: rpr/stack.c rpr/stack.h
	gcc $(CCOPTS) $(LDOPTS) -c rpr/stack.c -o rpr/stack.o

rpr/expr.o: rpr/expr.c rpr/expr.h
	gcc $(CCOPTS) $(LDOPTS) -c rpr/expr.c -o rpr/expr.o

rpr/core.o: rpr/core.c rpr/core.h
	gcc $(CCOPTS) $(LDOPTS) -c rpr/core.c -o rpr/core.o

$(TEST_C): tests/*.check
	checkmk tests/*.check > $(TEST_C)

tests: $(TEST_C) $(LIB)
	$(CC) $(CCOPTS) $(TEST_C) -o $(TEST_APP) $(LDOPTS) $(LIBS) -l$(LIB)
	-CK_TIMEOUT_MULTIPLIER=10 ./$(TEST_APP)

check: $(SOURCES) graph.c $(HEADERS) graph.h
	cp $(ROOT)/materials/linters/.clang-format ./
	clang-format -i $(SOURCES)
	clang-format -i $(HEADERS)
	clang-format -i graph.c graph.h
	clang-format -i gtk.c gtk.h
	clang-format -n $(SOURCES)
	clang-format -n $(HEADERS)
	clang-format -n graph.c graph.h
	clang-format -n gtk.c gtk.h

check2: $(SOURCES) graph.c $(HEADERS) graph.h
	cppcheck --enable=all --suppress=missingIncludeSystem --suppress=unusedFunction $(ROOT)/src/

.PHONY: all clean rebuild

clean:
	rm -rf $(OBJECTS) $(OBJECTS:.o=.c.bak) $(OBJECTS:.o=.h.bak) $(APP) lib$(LIB).a $(TEST_APP) $(TEST_C) $(RELEASE_NAME).tar.gz $(RELEASE_NAME)
	rm -rf *.o $(TEST_APP) lib$(LIB).a $(OBJECTS) *.a *.gcno *.gcda *.gcov *.info CPPLINT.cfg .clang-format *.app *.log *.aux *.dvi *.toc

rebuild: clean all

leaks: $(APP)
	$(LEAKS) ./$(APP) $(APP_ARGS)

leaks_test: test_leaks

test_leaks: tests
	$(LEAKS) ./$(TEST_APP) $(APP_ARGS)

gcov_report: clean $(TEST_C)
	$(CC) $(CCOPTS) --coverage -c $(SOURCES)
	ar rc lib$(LIB).a *.o
	$(CC) $(CCOPTS) $(TEST_C) -L./ -l$(LIB) $(LIBS) -lgcov -o $(TEST_APP)
	-CK_TIMEOUT_MULTIPLIER=5 ./$(TEST_APP)
	gcov *.gcda
	lcov -t "gcov_report" -o gcov_report.info -c -d .
	[ -d $(ROOT)/src/report ] || mkdir $(ROOT)/src/report
	genhtml -o $(ROOT)/src/report gcov_report.info
	rm -rf *.o $(TEST_APP) lib$(LIB).a $(OBJECTS) *.a *.gcno *.gcda *.gcov *.info $(TEST_C)

docker_build: clean
ifeq ($(OS), Linux)
		rm -rf report
		sudo docker build . -t $(DOCKER_NAME)
endif

docker_run: clean
ifeq ($(OS), Linux)
		rm -rf report
		sudo docker build . -t $(DOCKER_NAME)
		sudo docker run -p 80:80 --rm $(DOCKER_NAME)
endif

docker_bash: clean
ifeq ($(OS), Linux)
		rm -rf report
		sudo docker build . -t $(DOCKER_NAME)
		sudo docker run --rm -it -p 80:80 --entrypoint bash $(DOCKER_NAME)
endif

broadway: gtk
	sudo ./run.sh