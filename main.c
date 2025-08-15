#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

int main(int argc, const char* argv[]) {
	LexerContext* ctx;
	if(MakeLexerCtx(&ctx, argv[1]) < 0) {
		printf("Failed to make lexer context\n");
		return 1;
	}

	Token* tok = NULL;
	while (LexSource(ctx, &tok)) {
		PrintToken(tok);
		if (GetTokenType(tok) == TOKEN_EOF)
			break;
		DestroyToken(tok);
	}

	DestroyToken(tok);
	DestroyLexerCtx(ctx);

	return 0;
}
