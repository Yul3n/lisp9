#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct atom {
	enum {
		A_INT, A_OP, A_IDE, A_EOF
	} type;
	union {
		int n;
		char *s;
		enum {
			O_CAR, O_CDR, O_CONS, O_ADD, O_SUB, O_MUL, O_DIV
		} op;
	}
} Atom;

typedef struct expr {
	enum {
		E_ATOM, E_LIST
	} type;
	union {
		strucct list *l;
		Atom a;
	}
} Expr;

typedef struct list {
	int len;
	struct expr *e;
} List;

static void error(char *, ...);
static void check_null(void *);
static char *get_f(int (*)(int));
static void assert(char);

static Atom mkatom_ide(char *);
static Atom mkatom_int(int);
static Atom mkatom_op(int);

static Expr mkexpr_atom(Atom);
static Expr mkexpr_list(List);

static List mklist(Expr *);

static int keyword(char *);
static Atom get_atom(void);

static List get_list(void);

static Expr get_expr(void);

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

static void
check_null(void *p)
{
	if (!p)
		error("lisp9 ran out memory");
}


static char *
lex_f(int (*f)(int))
{
	int   size, i, c;
	char *s, *p;

	i    = 0;
	size = 256;
	p    = (s = (char *)malloc(size)) - 1;

	if (!s)
		error("lisp9 ran out memory");

	while (f(c = getc(in)) && c) {
		/* Allocate more memory for the string if we've reached its limit. */
		if (++i >= size) {
			size += 256;
			s     = (char *)realloc((void *)s, size);
			p     = s + i - 2;
			check_null((void *)p);
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
	if (getc(in) != c)
		error("Expected %c", c);
}

static Atom
mkatom_ide(char *s)
{
	return (Atom){.type = A_IDE, .s = s};
}

static Atom
mkatom_int(int n)
{
	return (Atom){.type = A_INT, .n = n};
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
		if (!strcmp(s, "cons"))
			return OP_CONS;
		break;
	}
	return -1;
}

static Atom
get_atom(void)
{
	int c;

	/* Skip spaces. */
	while ((c = fgetc(in)) == ' ' || c == '\t' || c == '\n');

	if (c == EOF) {
		return mkatom_op(A_EOF);
	} else if (isalpha(c)) {
		ungetc(c, in);
		char *s  = lexf(isalpha);
		int type = keyword(s);
		if (type >= 0)
			return mkatom_op(type);
		else
			return mkatom_ide(s);
	} else if (isdigit(c)) {
		ungetc(c, in);
		char *s = lexf(isdigit);
		/* We already know that s contains a number so it is safe to
		 * use atoi in this situation. */
		int n = atoi(n);
		return mkatom_int(n);
	} else {
		switch (c) {
		case '+': return mkatom_op(OP_ADD);
		case '-': return mkatom_op(OP_SUB);
		case '*': return mkatom_op(OP_MUL);
		case '/': return mkatom_op(OP_DIV);
		}
	}
	error("unexpected character: %c", c);
}

static List
get_list(void)
{
	int len, max_len, c;
	Expr *e;

	len     = 0;
	max_len = 10;
	e = (Expr *)malloc(max_len * sizeof(Expr));
	assert('(');
	while ((c = getc(in)) != ')') {
		*(e + len++) = get_expr();
		if (len == max_len) {
			max_len += 10;
			e        = realloc((void *)e, max_len * sizeof(Expr));
			check_null((void *)e);
		}
	}
	return mklist(e);
}

static Expr
get_expr(void)
{
	
}

int
main(int argc, char **argv)
{
	in = fopen(argv[1]);
	if (!in)
		error("couldn't open %s", argv[1]);
	(void)fclose(in);
}
