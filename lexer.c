#include <stdio.h>
#include "lexer.h"

struct Token {
	int line;
	int pos;
	const char* file;
	union {
		int n;
		char* d;
		char c;
	} value;
	int type;
};

struct LexerContext {
	FILE* file;
	const char* fname;
	int line;
	int pos;
	long flags;
};

int LexSource(LexerContext* file, Token* ret) {
	return 0;
}

