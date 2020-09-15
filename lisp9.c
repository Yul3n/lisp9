#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct atom {
	enum {
		A_INT, A_STR, A_OP, A_IDE
	} type;
	union {
		int n;
		char *s;
		enum {
			O_CAR, O_CDR, O_CONS, O_ADD, O_SUB, O_MUL, O_DIV
		} op;
	}
} Atom;

typedef struct list {
	enum {
		L_ATOM, L_LIST
	} type;
	struct list *l;
	Atom a;
} List;

static void error(char *, ...);
static char *get_f(int (*)(int));
static void assert(char);

static Atom mkatom_ide(char *);
static Atom mkatom_op(int);

static int keyword(char *);
static Atom get_atom(void);
static List get_expr(void);

FILE *in;

static void
error(char *s, ...)
{
	va_list args;
	char    buf[512];

	va_start(args, s);
	vsnprintf(buf, 512, s, args);
	va_end(args);
	printf("error: %s\n", buf);
	exit(1);
}

static char *
lex_f(int (*f)(int))
{
	int   size, i, c;
	char *s, *p;

	i    = 0;
	size = 256;
	p    = (s = malloc(size)) - 1;

	if (!s)
		error("lisp9 ran out memory");

	while (f(c = getc(in)) && c) {
		/* Allocate more memory for the string if we've reached its limit. */
		if (++i >= size) {
			size += 256;
			s     = realloc(s, size);
			p     = s + i - 2;
			if (!s)
				error("lisp9 ran out memory");
		}
		*(++p) = c;
	}
	/* NUL-terminate the string. */
	s[i] = 0;
	ungetc(c, in);
	return s;
}

static void
assert(char c)
{
	int nc;
	if (next_char() != c)
		error();
}

static Atom
mkatom_ide(char *s)
{
	return (Atom){.type = A_IDE, .s = s};
}
static Atom
mkatom_op(int op)
{
	return (Atom){.type = A_OP, .op = op};
}

static int
keyword(char *s)
{
	switch (*s) {
	case 'c':
		if (!strcmp(s, "cdr"))
			return OP_CDR;
		if (!strcmp(s, "car"))
			return OP_CAR;
		break;
	}
	return -1;
}

static Atom
get_atom(void)
{
	int c;

	c = fgetc(in);
	if (isalpha(c)) {
		ungetc(c, in);
		char *s  = lexf(isalpha);
		int type = keyword(s);
		if (type >= 0)
			return mkatom_op(type);
		else
			return mkatom_ide(s);
	} else if (isdigit(c)) {
		
	} else {
		
	}
}

int
main(int argc, char **argv)
{
	in = fopen(argv[1]);
	if (!in)
		error("couldn't open %s", argv[1]);
}
