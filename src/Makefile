LDOPTS 				= -L.
CCOPTS 				= -Wall -Werror -Wextra -DNDEBUG -g
GTK_OPTS 			= -g -rdynamic `pkg-config --cflags gtk+-3.0`
GTK_LIBS 			= `pkg-config --libs gtk+-3.0`
CC 					= g++
APP_ARGS 			= "sin(cos(x^2)^(1*-100))*x"
TEST_APP 			= test.app
TXT_APP 			= console.app
TXT_C 				= consoleapp.cc consoleview/consoleview.cc
TXT_O 				= $(TXT_C:.cc=.o)
TXT_H 				= consoleview/consoleview.h controllers/examplecontroller.h 
MODELS				= models/*.h 
CONTROLLERS			= controllers/*.h
VIEWS				= consoleview/*.h consoleview/*.cc
NEW_TEST_C 			= tests/new_test.cc models/*.cc
DOCKER_NAME			= calc:2.01
RELEASE_NAME		= s21_SmartCalc-2.01
TEXI2DVI			= texi2dvi
DOC_NAME			= doc
CLEAN				= rm -rf
.SUFFIXES:
.SUFFIXES: .cc .o
ROOT 				:= ../
GCOV				= -fprofile-arcs -ftest-coverage
OS					= $(shell uname)
ifeq ($(OS), Linux)
	CHECK_LIBS		= $(shell pkg-config --libs gtest) -lgtest -lm
	LIBS			= -lcheck -lpthread -lrt -lm -lsubunit
	LEAKS			= CK_FORK=no valgrind -s --tool=memcheck --leak-check=full --show-leak-kinds=all --show-reachable=yes 
else
	CHECK_LIBS		= -lgtest -lm
	LIBS			= -lcheck
	LEAKS			= CK_FORK=no leaks --atExit -- 
endif

LIB 				= s21_SmartCalc
SRC_DIR				= $(ROOT)/src
OBJECTS 			= rpn_cpp/expr.o rpn_cpp/core.o
SOURCES 			= $(OBJECTS:.o=.cc)
HEADERS 			= $(OBJECTS:.o=.h)
INST_DIR			= $(ROOT)/build
NAME				= Calc_2_0

all:  install

.cc.o:
	$(CC) -c $(CCOPTS) -o $@ $<


dvi: $(DOC_NAME).dvi

$(DOC_NAME).dvi: $(DOC_NAME).texi
	$(TEXI2DVI) $(SRC_DIR)/$(DOC_NAME).texi

dist: clean $(SOURCES)
	find Calc_2_0/ -type d | grep -vE "(QCP)|(Calc_2_0/$$)" | xargs $(CLEAN)
	[ -d $(RELEASE_NAME) ] || mkdir $(RELEASE_NAME)
	cp -r ./ ../$(RELEASE_NAME)/
	tar -czf $(RELEASE_NAME).tar.gz ../$(RELEASE_NAME)
	# rm -rf ../$(RELEASE_NAME)
	
install: lib
	qmake CONFIG+=release -o $(INST_DIR)/Makefile $(NAME)/$(NAME).pro
	-cd $(INST_DIR); make
	-cd $(INST_DIR); make clean
	$(CLEAN) $(INST_DIR)/Makefile
	$(CLEAN) $(INST_DIR)/.qmake.stash
	$(CLEAN) $(INST_DIR)/qmake_qmake_qm_files.qrc
	$(CLEAN) $(INST_DIR)/.qm

uninstall: clean
	$(CLEAN) $(INST_DIR)	

lib: $(LIB)

$(LIB): lib$(LIB).a

lib$(LIB).a: $(OBJECTS)
	ar rcs lib$(LIB).a $(OBJECTS)

$(TXT_APP): clean $(TXT_O) $(TXT_H) $(LIB) 
	$(CC) $(CCOPTS) $(TXT_O) -o $@ $(LDOPTS) -l$(LIB) $(LDOPTS)

tests: clean $(NEW_TEST_C) $(LIB)
	$(CC) $(CCOPTS) $(NEW_TEST_C) -o $(TEST_APP) $(LDOPTS) $(CHECK_LIBS) -l$(LIB)
	-CK_TIMEOUT_MULTIPLIER=10 ./$(TEST_APP)

check: $(SOURCES) $(HEADERS)
	cp $(ROOT)/materials/linters/.clang-format ./
	clang-format -i $(SOURCES)
	clang-format -i $(HEADERS)
	clang-format -i $(NEW_TEST_C)
	clang-format -i $(TXT_C)
	clang-format -i $(TXT_H)
	clang-format -i $(MODELS)
	clang-format -i $(CONTROLLERS)
	clang-format -i $(VIEWS)
	clang-format -n $(SOURCES)
	clang-format -n $(HEADERS)
	clang-format -n $(NEW_TEST_C)
	clang-format -n $(TXT_C)
	clang-format -n $(TXT_H)
	clang-format -n $(MODELS)
	clang-format -n $(CONTROLLERS)
	clang-format -n $(VIEWS)
	clang-format -n Calc_2_0/*.cc
	clang-format -n Calc_2_0/*.h
check2: 
	cppcheck --std=c++17 --enable=all --language=c++ \
	--suppress=missingInclude --suppress=unusedFunction \
	--suppress=unknownMacro --suppress=knownConditionTrueFalse \
	-i $(ROOT)src/Calc_2_0/QCP -i $(ROOT)src/build* $(ROOT)src/

.PHONY: all clean rebuild check2

clean:
	rm -rf $(OBJECTS) $(TXT_O) $(OBJECTS:.o=.c.bak) $(OBJECTS:.o=.h.bak) $(APP) lib$(LIB).a $(TEST_APP) $(TEST_C) $(RELEASE_NAME).tar.gz $(RELEASE_NAME) ../$(RELEASE_NAME)
	rm -rf *.o $(TEST_APP) lib$(LIB).a $(OBJECTS) *.a *.gcno *.gcda *.gcov *.info CPPLINT.cfg .clang-format *.app *.log *.aux *.dvi *.toc report

rebuild: clean uninstall all

leaks: $(TXT_APP)
	$(LEAKS) ./$(TXT_APP) $(APP_ARGS)

leaks_test: test_leaks

test_leaks: tests
	$(LEAKS) ./$(TEST_APP) $(APP_ARGS)

gcov_report: clean $(NEW_TEST_C)
	$(CC) $(CCOPTS) --coverage -c $(SOURCES)
	ar rc lib$(LIB).a *.o
	$(CC) $(CCOPTS) --coverage $(NEW_TEST_C) -L./ -l$(LIB) $(LIBS) $(LDOPTS) $(CHECK_LIBS)  -lgcov -o $(TEST_APP)
	-CK_TIMEOUT_MULTIPLIER=5 ./$(TEST_APP)
	gcov *.gcda
	lcov -t "gcov_report" -o gcov_report.info -c -d .
	[ -d $(ROOT)/src/report ] || mkdir $(ROOT)/src/report
	genhtml -o $(ROOT)/src/report gcov_report.info
	rm -rf *.o $(TEST_APP) lib$(LIB).a $(OBJECTS) *.a *.gcno *.gcda *.gcov *.info $(TEST_C)
