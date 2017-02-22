all: lexer parser sema


LLVMCONFIG = llvm-config
CPPFLAGS = `$(LLVMCONFIG) --cppflags` -std=c++11
LDFLAGS = `$(LLVMCONFIG) --ldflags --system-libs --libs ` -lpthread -ldl #-lz -lncurses -rdynamic

lexer:
	flex -o tokens.cpp tokens.l

parser:
	bison parser.y -d -o parser.cpp
	g++ -c $(CPPFLAGS) parser.cpp -o parser.o
	g++ -c $(CPPFLAGS) tokens.cpp -o tokens.o

sema:
	g++ -c $(CPPFLAGS) main.cpp -o main.o
	g++ -c $(CPPFLAGS) codegen.cpp -o codegen.o
	g++ -o parser parser.o main.o tokens.o codegen.o $(LDFLAGS)

test:
	cat test.simple.txt | ./parser

clean:
	rm -rf parser.cpp parser.hpp parser tokens.cpp *.o
