#ifndef _LEXER_H_
#define _LEXER_H_

struct Token;
typedef struct Token Token;

struct LexerContext;
typedef struct LexerContext LexerContext;

int  LexSource(LexerContext* source, Token* ret);
void PrintToken(Token* tok);

#endif
