#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct atom {
	enum {
		A_INT, A_STR, A_OP, A_IDE
	} type;
	int n;
	char *s;
	enum {
		O_CAR, O_CDR, O_CONS, O_ADD, O_SUB, O_MUL, O_DIV
	} op;
} Atom;

typedef struct list {
	enum {
		L_ATOM, L_LIST
	} type;
	struct list *l;
	Atom a;
} List;

Atom get_atom(void);
List get_expr(void);

int
main(int argc, char **argv)
{
	
}
