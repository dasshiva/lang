#ifndef _LANG_H_
#define _LANG_H_

void error(char* what, char* line, int line);

enum TokenType {
  // Single-character tokens.
 LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,

 COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,
 // One or two character tokens.
 BANG, BANG_EQUAL,
 EQUAL, EQUAL_EQUAL,
 GREATER, GREATER_EQUAL,
 LESS, LESS_EQUAL,
 // Literals.
 IDENTIFIER, STRING, NUMBER,
 // Keywords.
 AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
 PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,
 EOF
}

struct Token {
  TokenType ty;
  char* lexeme;
  void* literal;
  int line;
}

Token* new_token(TokenType tk, char* lexeme, void* literal, int line);
void* tk_to_string(Token* token);
#endif