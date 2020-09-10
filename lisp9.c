#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static void error(char *, ...);
static int next_char(void);
static void putback_char(int);
static char *get_f(int (*)(int));
static void assert(char);
static int keyword(char *);
static Atom get_atom(void);
static List get_expr(void);

FILE *in;
int unused_char = '\0';

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

static int
next_char(void)
{
	int c;
	if (unused_char == '\0')
		c = fgetc(in);
	else {
		c = unused_char;
		unused_char = '\0';
	}
	return c;
}

static void
putback_char(char c)
{
	unused_char = c;
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

	while (f(c = next_char(file)) && c) {
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
	putback_char(c);
	return s;
}

static void
assert(char c)
{
	int nc;
	if (next_char() != c)
		error)
}

int
main(int argc, char **argv)
{
	in = fopen(argv[1]);
	if (!in)
		error("couldn't open %s", argv[1]);
}
