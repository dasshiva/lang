#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

int main(int argc, const char* argv[]) {
	FILE* file = fopen(argv[1], "r");
	if (!file) {
		printf("Error accessing source file\n");
		return 1;
	}

	Token* tok = NULL;
	while (LexSource(file, tok)) {
		PrintToken(tok);
	}

	fclose(file);
	return 0;
}
