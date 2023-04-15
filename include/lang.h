#ifndef _LANG_H_
#define _LANG_H_

#include <sds/sds.h>
#include <c_arraylist/arraylist.h>
void error(char*, char*, int);

typedef enum {
  // Single-character tokens.
 LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,

 COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,
 // One or two character tokens.
 BANG, BANG_EQUAL,
 EQUAL, EQUAL_EQUAL,
 GREATER, GREATER_EQUAL,
 LESS, LESS_EQUAL,
 // Literals.
 IDENTIFIER, STRING, NUMBER, FLOAT,
 // Keywords.
 AND = 20, CLASS = 21, ELSE = 22, FALSE = 23,
 FUN = 24, FOR = 25, IF = 26, NIL = 27, OR = 28,
 PRINT = 28, RETURN = 29, SUPER = 30, THIS = 31, TRUE = 32, VAR = 33, WHILE = 34,
 END
} TokenType;

typedef struct {
  TokenType ty;
  sds lexeme;
  void* literal;
  int line;
} Token;

Token* new_token(TokenType tk, char* lexeme, void* literal, int line);
void tk_to_string(Token* token);

typedef struct {
  sds src;
  arraylist* list;
  int start;
  int current;
  int line;
} Lexer;

Lexer* new_lexer(char* src);
#endif
