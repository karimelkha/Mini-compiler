%{
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "defs.h"
#include "common.h"

#include "y.tab.h"

int yyparse(node_t * program_root);
void analyse_tree(node_t root);


char * infile = NULL;
char * outfile = DEFAULT_OUTFILE;
bool stop_after_syntax = false;
bool stop_after_verif = false;

#if YYDEBUG
extern int yydebug;
#endif


%}

%option yylineno


LETTRE          [a-zA-Z]
CHIFFRE 		[0-9]
/* A completer ; fausses definitions pour passer la compilation */

IDF             {LETTRE}({LETTRE}|{CHIFFRE}|'_')*
CHIFFRE_NON_NUL [1-9]
ENTIER_DEC		0|({CHIFFRE_NON_NUL}{CHIFFRE}*)
LETTRE_HEXA 	[a-fA-F]
ENTIER_HEXA		'0x'{CHIFFRE}|{LETTRE_HEXA}+
ENTIER          {ENTIER_DEC}|{ENTIER_HEXA}
CHAINE_CAR		[^\042\134]
CHAINE          \042({CHAINE_CAR}|\134|\\n)*\042
COMMENTAIRE     "//".*\n


%%

"void"          return TOK_VOID;
"int"           return TOK_INT;
"bool"          return TOK_BOOL;
"true"          return TOK_TRUE;
"false"         return TOK_FALSE;
"if"            return TOK_IF;
"else"          return TOK_ELSE;
"while"         return TOK_WHILE;
"for"           return TOK_FOR;
"do"            return TOK_DO;
"print"         return TOK_PRINT;
";"             return TOK_SEMICOL;
"="             return TOK_AFFECT;
"("             return TOK_LPAR;
")"             return TOK_RPAR;
"{"             return TOK_LACC;
"}"             return TOK_RACC;
","             return TOK_COMMA;
"<"             return TOK_LT;
">"             return TOK_GT;
"+"             return TOK_PLUS;
"-"             return TOK_MINUS;
"*"             return TOK_MUL;
"/"             return TOK_DIV;
"%"             return TOK_MOD;
"!"             return TOK_NOT;
"~"             return TOK_BNOT;
"&"             return TOK_BAND;
"|"             return TOK_BOR;
"^"             return TOK_BXOR;
">>"            return TOK_SRL;
"<<"            return TOK_SLL;
">>>"           return TOK_SRA;
"<="            return TOK_LE;
">="            return TOK_GE;
"=="            return TOK_EQ;
"!="            return TOK_NE;
"&&"            return TOK_AND;
"||"            return TOK_OR;


{IDF} {
                yylval.strval = strdup(yytext);
                return TOK_IDENT;
}

{ENTIER} {
    // TODO entiers nuls les trucs comme ac
                yylval.intval = atoi(strdup(yytext));
                return TOK_INTVAL;
}

{CHAINE} {
                yylval.strval = strdup(yytext);
                
                return TOK_STRING;
}



{COMMENTAIRE} {}






[ \t\n]+

. {
                fprintf(stderr, "Error line %d: Lexical error\n", yylineno);
                exit(1);
}


%%

int yywrap(void) {
    return 1;
}



int main(int argc, char ** argv) {
    node_t program_root;
    parse_args(argc, argv);
    #if YYDEBUG
    yydebug = 1;
    #endif
    
    yyin = fopen(infile, "r");
    yyparse(&program_root);
    fclose(yyin);
    analyse_tree(program_root);
    yylex_destroy();
    return 0;
}




