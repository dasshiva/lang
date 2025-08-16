#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "error.h"

// 1 '\t' is interpreted as this many spaces
#define SPACES_PER_TAB (4)

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

void LexerError();

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

// LexSource - Fetch the next token from the file or the
// pushback buffer. If not fetching from the pushback buffer
// also advance the file pointer to just behind the next 
// possible token.
//
// We define a token to be anything other than EOF or any
// whitespace character
//
// Parameters:
// @ctx - The lexer context that we are currently using to fetch
// tokens from
// @ret - The buffer into which the new token will be returned
// into (Must be non-null and should be freed with @DestroyToken
// after use)
// 
// Returns:
// OK - A token was succesfully fetched
// EARGNULL - ctx and/or ret was NULL
// EMEMALLOC - could not allocate memory for the token
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
		if (c > 127) {
			// All files must be encoded in US-ASCII only
			// LexerError();
		}

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
		
		switch (c) {
			case ' ': 
			case '\r':
			case '\f': ctx->pos += 1; continue;
			case '\t': ctx->pos += SPACES_PER_TAB; continue;
			case '\v': ctx->line += 1; continue;
			case '\n': ctx->line += 1; ctx->pos = 1; continue;
			default: {}
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

int GetTokenType(Token* tok) {
	return tok->type;
}

void LexerError() {
	exit(1);
}

