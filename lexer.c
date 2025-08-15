#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "error.h"

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

int MakeLexerCtx(LexerContext* ctx, const char* name) {
	if (!name)
		return EARGNULL;

	ctx = (LexerContext*) malloc(sizeof(LexerContext));
	if (!ctx)
		return EMEMALLOC;

	ctx->file = fopen(name, "r");
	if (!ctx->file)
		return EFILE;

	ctx->fname = name;
	ctx->line = 1;
	ctx->pos = 1;
	ctx->flags = 0;
	return 1;
}

int LexSource(LexerContext* ctx, Token* ret) {
	return 1;
}

