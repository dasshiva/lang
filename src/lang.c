#include <stdlib.h>
#include <include/lang.h>

void error(char* what, char* code, int line) {
	printf("In line %d - %s\n", line, code);
	printf("Error :%s", what);
	exit(1);
}

static void RunFile(char* name) {
	FILE* file = fopen(name, "rb");
	if (!file) {
		printf("Can't open %s", name);
		exit(1);
	}
	fseek(file, 0, SEEK_END);
	long end = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* buf = malloc(sizeof(char) * end);
	fread(buf, end, sizeof(char), file);
	fclose(file);
	Lexer* lex = new_lexer(buf);
	scan_tokens(lex);
}

int main(int argc, char *args[]) {
    if (argc > 2) {
	    printf("Usage lang [file]\n");
	    printf("Need filename!");
	    exit(64);
    }
    else if (argc == 2) {
	    RunFile(args[1]);
    }
    else {
	    //RunPrompt();
    }
    return 0;
}
