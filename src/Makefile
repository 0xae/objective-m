DEBUG=-g -DDEBUG -Wall
OPTS=-std=c++11
BIN=../bin
TESTS=../tests

all: tests objm

tests: core core-tests
	$(TESTS)/test_atom && $(TESTS)/test_list && $(TESTS)/test_string
core: nobject nhandler nutils parser
core-tests: test-atom test-list test-string
clean:
	rm *.o
	rm $(TESTS)/*
	rm $(BIN)/*


## core components
nobject:
	g++ $(OPTS) $(DEBUG) -c nobject.cpp
nhandler:
	g++ $(OPTS) $(DEBUG) -c nhandler.cpp
nutils:
	g++ $(OPTS) $(DEBUG) -c nutils.cpp
parser:
	g++ $(OPTS) $(DEBUG) -c parser.cpp


## some tests
test-atom:
	g++ $(OPTS) $(DEBUG) -o $(TESTS)/test_atom nutils.o parser.o nobject.o nhandler.o test_atom.cpp
test-list:
	g++ $(OPTS) $(DEBUG) -o $(TESTS)/test_list nutils.o parser.o nobject.o nhandler.o test_list.cpp
test-string:
	g++ $(OPTS) $(DEBUG) -o $(TESTS)/test_string nutils.o parser.o nobject.o nhandler.o test_string.cpp


## objective-m interpreter
objm:
	g++ $(OPTS) $(DEBUG) -o $(BIN)/objm nutils.o parser.o nobject.o nhandler.o objectivem.cpp



