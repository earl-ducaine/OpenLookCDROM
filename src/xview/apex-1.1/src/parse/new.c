#ifndef lint
static char    *RCSid = "$Header: /n/homeserver/i/gounares/pu/apex/src/parse/RCS/new.c,v 1.1 93/01/06 03:46:03 gounares Exp Locker: gounares $";

#endif

/*
 * $Log:	new.c,v $
 * Revision 1.1  93/01/06  03:46:03  gounares
 * Initial revision
 * 
 */

/*
 * Copyright 1993 Alexander Gounares
 * 
 * This source is covered by the GNU General Public License Version 2
 * 
 * see the apeX manual for more details
 */

#include <stdio.h>
#include "y.tab.h"
#include <assert.h>
#include <math.h>

extern char     yytext[];
extern char     yyfilename[];
extern int      yylineno;
extern FILE    *yyin;

#define HASHSIZE 211

typedef struct _ID {
	char            szName[256];
	struct _ID     *next;
}               ID;

static unsigned int 
hash(s)
    char           *s;
{
	char           *p;
	unsigned        h = 0,
	                g;

	for (p = s; *p != '\0'; p++) {
		h = (h << 4) + (*p);
		if (g = h & 0xf0000000) {
			h ^= (g >> 24);
			h ^= g;
		}
	}
	return h % HASHSIZE;
}

static void 
clear_table(table)
    ID            **table;
{
	int             i;
	ID             *ptable,
	               *pnext;

	for (i = 0; i < HASHSIZE; i++) {
		for (ptable = table[i]; ptable != NULL; ptable = pnext) {
			pnext = ptable->next;
			free(ptable);
		}
		table[i] = NULL;
	}
}

static ID      *
strsave(table, szName)
    ID            **table;
    char           *szName;
{
	int             h = hash(szName);
	ID             *ptable,
	              **prev;

	for (ptable = table[h], prev = &table[h];
		ptable != NULL;
		prev = &(ptable->next), ptable = ptable->next)
		if (!strcmp(ptable->szName, szName))
			return NULL;

	/* if we got there, then this is a new entry */

	ptable = (ID *) calloc(1, sizeof(*ptable));
	assert(ptable);

	*prev = ptable;
	strcpy(ptable->szName, szName);
	return ptable;
}

static char     gops[315];

static int 
is_op(t)
    int             t;
{
	if (t == LEFT_ASSIGN ||
		t == RIGHT_ASSIGN ||
		t == ADD_ASSIGN ||
		t == SUB_ASSIGN ||
		t == MUL_ASSIGN ||
		t == DIV_ASSIGN ||
		t == MOD_ASSIGN ||
		t == AND_ASSIGN ||
		t == XOR_ASSIGN ||
		t == OR_ASSIGN ||
		t == RIGHT_OP ||
		t == LEFT_OP ||
		t == LEFT_OP ||
		t == INC_OP ||
		t == DEC_OP ||
		t == PTR_OP ||
		t == AND_OP ||
		t == OR_OP ||
		t == LE_OP ||
		t == GE_OP ||
		t == EQ_OP ||
		t == NE_OP ||
		t == '=' ||
		t == '&' ||
		t == '!' ||
		t == '~' ||
		t == '-' ||
		t == '+' ||
		t == '*' ||
		t == '/' ||
		t == '%' ||
		t == '<' ||
		t == '>' ||
		t == '^' ||
		t == '|')
		return (gops[t] = 1);
	return 0;
}

static int 
num_unique_ops()
{
	int             i,
	                count = 0;

	for (i = 0; i < 315; i++) {
		if (gops[i])
			count++;
		gops[i] = 0;
	}
	return count;
}


static void 
handle_function(szFuncname, szFilename, lineno, fStatic)
    char           *szFuncname;
    char           *szFilename;
    int             lineno;
    int             fStatic;
{
	int             scope = 1,
	                id_count = 0,
	                id_ucount = 0,
	                op_count = 0,
	                call_count = 0,
	                t,
	                op_ucount;
	static ID      *table[HASHSIZE];
	ID             *pcall,
	               *phead = NULL;
	ID            **ppnext = &phead;
	int             V;

	while (scope) {
		t = yylex();
		if (is_op(t)) {
			op_count++;
			continue;
		}
		switch (t) {
		case '{':
			scope++;
			break;
		case '}':
			scope--;
			break;
		case IDENTIFIER:
			id_count++;
			if ((pcall = strsave(table, yytext))) {
				id_ucount++;
				while ((t = yylex()) == ')');
				if (t == '(') {
					/* we have a function call */
					call_count++;
					*ppnext = (ID *) calloc(1,
						sizeof(ID));
					strcpy((*ppnext)->szName,
						pcall->szName);
					ppnext = &((*ppnext)->next);
				}
				if (t == '{')
					scope++;
				else if (t == '}')
					scope--;
			}
			break;
		default:
			break;
		}
	}

	/* calculate metrics now... */

	op_ucount = num_unique_ops();
	if (id_ucount + op_ucount <= 1)
		V = 0;
	else
#ifdef hpux
		V = (int) ((double) (op_count + id_count) * 
                     M_LOG2E*log((double) (id_ucount + num_unique_ops())));
#else  hpux
		V = (int) ((double) (op_count + id_count) * log2(
				(double) (id_ucount + num_unique_ops())));
#endif hpux

	/* now dump out the information */

	printf("%c %s %s %d %d %d\n", (fStatic) ? 'S' : 'G', szFuncname,
		szFilename, lineno, V, call_count);

	for (pcall = phead; pcall != NULL;) {
		phead = pcall;
		printf("%s\n", pcall->szName);
		pcall = pcall->next;
		free(phead);
	}

	clear_table(table);
}



void 
eat_args()
{
	int             t;

	while ((t = yylex()) != ')')
		if (t == '(')
			eat_args();
}

static void 
parse_file()
{
	int             t,
	                pushback = 0;
	int             scope = 0,
	                fStatic = 0;
	int             lineno;
	char            szBuf[1024],
	                szFunc[1024],
	                szFilename[1024];

	while ((t = pushback) || (t = yylex())) {
		switch (t) {
		case '{':
			scope++;
			pushback = 0;
			break;
		case '}':
			scope--;
			pushback = 0;
			break;
		case STATIC:
			pushback = 0;
			fStatic = 1;
			break;
		case ';':
			pushback = 0;
			fStatic = 0;
			break;
		case IDENTIFIER:
			strcpy(szBuf, yytext);
			strcpy(szFilename,
				(yyfilename[0] == '\"') ?
				yyfilename + 1 : yyfilename);
			if (szFilename[strlen(szFilename) - 1] == '\"')
				szFilename[strlen(szFilename) - 1] = '\0';
			lineno = yylineno;
			while ((t = yylex()) == ')');
			if (t == '(') {
				eat_args();
				if ((t = yylex()) != ';' && t != ',' && !scope) {
					strcpy(szFunc, szBuf);
					if (t != '{')
						while ((t = yylex()) != '{');
					handle_function(szBuf, szFilename, lineno, fStatic);
					fStatic = 0;
					scope--;
				}
			}
			pushback = t;
			break;
		default:
			pushback = 0;
			break;
		}
	}
}

main(argc, argv)
    int             argc;
    char          **argv;
{
	if (argc == 1) {
		parse_file();
		exit(0);
	}
	for (argc--; argc > 0; argc--) {
		yyin = fopen(argv[argc], "r");
		if (!yyin) {
			fprintf(stderr, "apeX parse: trouble opening file '%s'\n",
				argv[argc]);
			fprintf(stderr, "\tskipping...\n");
			continue;
		}
		strcpy(yyfilename, argv[argc]);
		parse_file();
	}

	exit(0);
}
