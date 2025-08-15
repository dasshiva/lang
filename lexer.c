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

int MakeLexerCtx(LexerContext** ctx, const char* name) {
	if (!name)
		return EARGNULL;

	*ctx = (LexerContext*) malloc(sizeof(LexerContext));
	if (!(*ctx)) {
		*ctx = NULL;
		return EMEMALLOC;
	}

	(*ctx)->file = fopen(name, "r");
	if (!(*ctx)->file)
		return EFILE;

	(*ctx)->fname = name;
	(*ctx)->line = 1;
	(*ctx)->pos = 1;
	(*ctx)->flags = 0;
	return OK;
}

int LexSource(LexerContext* ctx, Token** ret) {
	if (!ctx)
		return EARGNULL;

	*ret = (Token*) malloc(sizeof(Token));
	if (!*ret)
		return EMEMALLOC;

	Token* tok = *ret;

	return OK;
}

void PrintToken(Token* tok) {
#ifndef RELEASE
	printf("Token type %d at %d:%d\n", tok->type, tok->line,
			tok->pos);
#endif
}

void DestroyLexerCtx(LexerContext* ctx) {
	// FIXME: This is a "just get over with it" solution
	// for a pretty severe bug. We should completely crash
	// if ctx == NULL and give some kind of indication
	// for debugging.
	if (!ctx)
		return;
	fclose(ctx->file);
	free(ctx);
}

void DestroyToken(Token* tok) {
	free(tok);
}
