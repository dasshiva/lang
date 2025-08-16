#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "error.h"

// 1 '\t' is interpreted as this many spaces
#define SPACES_PER_TAB (4)

#define LEXER_EOF       (1 << 0)
#define LEXER_BLANK     (1 << 1)
#define LEXER_IN_TOKEN  (1 << 2)
#define LEXER_NEW_TOKEN (1 << 3)
#define LEXER_NEW_LINE  (1 << 4)

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
	int prev;
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
	(*ctx)->flags |= LEXER_BLANK;
	(*ctx)->saved = NULL;
	(*ctx)->prev = 1;
	return OK;
}

void SkipSpace(LexerContext* ctx) {
	if (!ctx)
		return;

	int c = 0;

	while (1) {
		c = fgetc(ctx->file);
		switch (c) {
			case ' ': 
			case '\r':
			case '\f': ctx->pos += 1; break;
			case '\t': ctx->pos += SPACES_PER_TAB; break;
			case '\v': ctx->line += 1; break;
			case '\n': {
				ctx->line += 1; 
				ctx->prev = ctx->pos;
				ctx->pos = 1;
				break;
			}
			default: {
				if (ctx->pos == 1) {
					ctx->pos = ctx->prev;
					ctx->flags |= LEXER_NEW_LINE;
					ctx->line -= 1;
				}
				else
					ctx->pos -= 1;
				ungetc(c, ctx->file); 
				return;
			}
		}
	}
}

int ReadAtom(LexerContext* ctx) {
	int c = fgetc(ctx->file);
	ctx->pos++;
	return c;
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
		// If we are starting out, there can be spaces at the
		// top of the file so skip them otherwise we are 
		// already pointing to the next token
		if (ctx->flags & LEXER_BLANK) {
			SkipSpace(ctx);
			ctx->flags &= ~LEXER_BLANK;
			continue;
		}

		c = ReadAtom(ctx);
		if (c > 127) {
			// All files must be encoded in US-ASCII only
			// More formally only characters from iso646-1991 
			// may be used. This prevents the lexer from being
			// overly complex to deal with encodings like UTF-8
			// allowing us to focus on other areas

			// There is however the obvious disadvantage that
			// users are forced to name all their program 
			// entities using ASCII only restricting them from
			// doing so in their native tongue
			
			LexerError();
			// Won't return
		}
		
		if (c == EOF) {
			// Return the last recognised token
			// The lexer will only return to the caller if
			// a) We hit EOF pack up your bags
			// b) It reaches the beginning of the next token
			// So this will never return the EOF token
			// Only the if() outside the loop does
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

int GetTokenType(Token* tok) {
	return tok->type;
}

void LexerError() {
	exit(1);
}

