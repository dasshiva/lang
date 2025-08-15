#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "error.h"

#define LEXER_EOF (1 << 0)

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
	Token* saved;
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
	(*ctx)->saved = NULL;
	return OK;
}

int LexSource(LexerContext* ctx, Token** ret) {
	if (!ctx)
		return EARGNULL;

	*ret = (Token*) malloc(sizeof(Token));
	if (!*ret)
		return EMEMALLOC;

	Token* tok = *ret;
	tok->file = ctx->fname;

	if (ctx->flags & LEXER_EOF) {
		tok->line = ctx->line;
		tok->pos = ctx->pos;
		tok->type = TOKEN_EOF;
		return OK;
	}

	int c = 0;

	while (1) {
		c = fgetc(ctx->file);
		if (c == EOF) {
			// Return the last recognised token
			// The lexer will only return to the caller if
			// a) We hit EOF pack up your bags
			// b) It reaches the beginning of the next token
			// So this will never return the EOF token
			// Only the if() above does
			ctx->flags |= LEXER_EOF;
			break;
		}
	}


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
