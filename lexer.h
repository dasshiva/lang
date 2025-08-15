#ifndef _LEXER_H_
#define _LEXER_H_

struct Token;
typedef struct Token Token;

struct LexerContext;
typedef struct LexerContext LexerContext;

int  MakeLexerCtx(LexerContext** ret, const char* name);
int  LexSource(LexerContext* source, Token** ret);
void PrintToken(Token* tok);
void DestroyLexerCtx(LexerContext* ret);
void DestroyToken(Token* tok);

enum TokenType {
	TOKEN_EOF = 1,
};

#endif
