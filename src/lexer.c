#include <include/lang.h>
#include <stdlib.h>

Token* new_token(TokenType tk, char* lexeme, void* literal, int line) {
  Token* token = malloc(sizeof(Token));
  token->ty = tk;
  token->lexeme = lexeme;
  token->literal = literal;
  token->line = line;
  return token;
}

void* tk_to_string(Token* token) {
  printf("%d %s\n", token->ty, token->lexeme);
}