#ifndef _LEXER_H_
#define _LEXER_H_

struct Token;
typedef struct Token Token;

int  LexSource(FILE* source, Token* ret);
void PrintToken(Token* tok);

#endif
