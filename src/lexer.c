#include <include/lang.h>
#include <stdlib.h>

static char* keywords[] = { "and", "class", "else", "false","for","fun", "if", "nil", "or", "print", "return", "super", "this", "true", "var","while" 
};

Token* new_token(TokenType tk, char* lexeme, void* literal, int line) {
  Token* token = calloc(1, sizeof(Token));
  token->ty = tk;
  token->lexeme = sdsnew(lexeme);
  token->literal = literal;
  token->line = line;
  return token;
}

void tk_to_string(Token* token) {
  printf("%d %s\n", token->ty, token->lexeme);
}

Lexer* new_lexer(char* src) {
  Lexer* lexer = calloc(1, sizeof(Lexer));
  lexer->src = sdsnew(src);
  lexer->list = arraylist_create();
  lexer->line = 1;
  return lexer;
}

static inline int end(Lexer* lexer) {
  return lexer->current >= sdslen(lexer->src);
}

static inline char advance(Lexer* lexer) {
  if (end(lexer))
    return '\0';
  return lexer->src[lexer->current++];
}

static void add_token( Lexer* lexer, TokenType tk) {
  arraylist_add(lexer->list, new_token(tk, "", NULL, lexer->line));
}

static inline int match (Lexer* lexer, char ex) {
  char c = advance(lexer);
  if (c == ex) {
    return 1;
  }
  else {
    lexer->current--;
    return 0;
  }
}

static char peek(Lexer* lexer) {
  char c = advance(lexer);
  lexer->current--;
  return c;
}

static char peekNext(Lexer* lexer) {
  if (lexer->current + 2 >= sdslen(lexer->src))
    return '\0';
  return lexer->src[lexer->current + 2];
}

static void string(Lexer* lexer, char delim) {
   while (peek(lexer) != delim && !end(lexer)) {
     if (peek(lexer) == '\n') lexer->line++;
     advance(lexer);
   }
   if (end(lexer)) {
     sdsrange(lexer->src, lexer->start, lexer->current); 
     error("Unexpected Token", lexer->src, lexer->line);
   }
   
   advance(lexer);
   sds lit = sdsdup(lexer->src);
   sdsrange(lit, lexer->start + 1, lexer->current - 1);
   arraylist_add(lexer->list, new_token(STRING, "", lit, lexer->line));
}

static void number(Lexer* lexer) {
  while (isdigit(peek(lexer))) advance(lexer);
  int frac = 0;
  if (peek(lexer) == '.' && isdigit(peekNext(lexer))) {
   frac = 1;
   advance(lexer);
   while (isdigit(peek(lexer))) advance(lexer);
  }
  sds lit = sdsdup(lexer->src);
  sdsrange(lit, lexer->start, lexer->current);
  char* a = NULL;
  if (!frac) {
      long num = strtol(lit, &a, 10);
      if (!a) {
        sdsrange(lexer->src, lexer->start, lexer->current); 
        error("Illegal integer number", lexer->src, lexer->line);
      }
      arraylist_add(lexer->list, new_token(NUMBER, "", (void*) num, lexer->line));
  }
  else {
    double num = strtod(lit, &a);
    if (!a) {
      sdsrange(lexer->src, lexer->start, lexer->current); 
      error("Illegal floating point number", lexer->src, lexer->line);
    }
    arraylist_add(lexer->list, new_token(FLOAT, "", (void*)((long) num), lexer->line));
  }
}

static void identifier(Lexer* lexer) {
  while (isalpha(peek(lexer)) || isdigit(peek(lexer))) advance(lexer);
  sds lit = sdsdup(lexer->src);
  sdsrange(lit, lexer->start, lexer->current);
  for (int i = 0; i < 15; i++) {
    if (strcmp(keywords[i], lit)) {
      arraylist_add(lexer->list, (new_token(i + 20, "", NULL, lexer->line)));
      return;
    }
  }
  add_token(lexer, IDENTIFIER);
}

static void scan(Lexer* lexer) {
  char c = advance(lexer);
  switch (c) {
    case '(': add_token(lexer,LEFT_PAREN); break;
    case ')': add_token(lexer,RIGHT_PAREN); break;
    case '{': add_token(lexer,LEFT_BRACE); break;
    case '}': add_token(lexer,RIGHT_BRACE); break;
    case ',': add_token(lexer,COMMA); break;
    case '.': add_token(lexer,DOT); break;
    case '-': add_token(lexer,MINUS); break;
    case '+': add_token(lexer,PLUS); break;
    case ';': add_token(lexer,SEMICOLON); break;
    case '*': add_token(lexer,STAR); break;
    case '!': add_token(lexer,match(lexer, '=') ? BANG_EQUAL : BANG); break;
    case '=': add_token(lexer,match(lexer, '=') ? EQUAL_EQUAL : EQUAL); break;
    case '<': add_token(lexer,match(lexer, '=') ? LESS_EQUAL : LESS); break;
    case '>': add_token(lexer,match(lexer, '=') ? GREATER_EQUAL : GREATER); break;
    case '/':
    if (match(lexer, '/')) {
      // A comment goes until the end of the line.
      while (peek(lexer) != '\n' && !end(lexer)) advance(lexer);
   } else {
     add_token(lexer, SLASH);
   }
    break;
    case ' ': case '\r': case '\t': break;
    case '"': case '\'': string(lexer, c); break;
    case '\n': lexer->line++;  break;
    default: 
    if (isdigit(c)) {
      number(lexer);
    }
    else if (isalpha(c)) {
      identifier(lexer);
    }
    else {
      sdsrange(lexer->src, lexer->start, lexer->current); 
      error("Unexpected Token", lexer->src, lexer->line);
    }
  }
}

void scan_tokens(Lexer* lexer) {
  while(!end(lexer)) {
    lexer->start = lexer->current;
    scan(lexer);
  }
  
  arraylist_add(lexer->list, (new_token(END, "", NULL, 1)));
}
