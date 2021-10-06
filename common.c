
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <inttypes.h>
#include <unistd.h>
#include <getopt.h>

#include "defs.h"
#include "common.h"
#include "arch.h"


extern char * infile;
extern char * outfile;
int32_t trace_level = DEFAULT_TRACE_LEVEL;
extern bool stop_after_syntax;
extern bool stop_after_verif;


void parse_args(int argc, char ** argv) {

    // options autorisé par la spécification
    char optstring[]="bo:t:r:svh";
    char chiffreT[] = "012345";
    char chiffreR[] = "45678";
    int c = 1;
    int cflag = 0;
    int hbflag = 0;

    // Verification s'il y a plusieurs fichiers c passé en argument et des arguments b et h
    for(int i = 1 ; i <= argc-1 ; i++){
        if(!strcmp(argv[i],"-h") || !strcmp(argv[i], "-b") ){
                hbflag = 1;
        }

        if(strstr(argv[i], ".c") != NULL){
            cflag++;
        }
    }

    if(argc==1 && !strcmp(argv[0], "./minicc")){
        printf("*******************************************************************************************************************\n\n  Usage: ./minicc <options> <infile>\n <infile>  : Input file to compile (mandatory)\n\n                  Liste des options\n\n   -b : Affiche une bannière indiquant le nom du compilateur et des membres du binôme.\n   -o <filename> : Définit le nom du fichier assembleur produit (défaut : out.s).\n   -t <int> : Définit le niveau de trace à utiliser entre 0 et 5 (0 = pas de trace; 5 = toutes les traces. defaut = 0).\n   -r <int> : Définit le nombre maximum de registres à utiliser, entre 4 et 8 (défaut : 8).\n   -s : Arrêter la compilation après l'analyse syntaxique (défaut = non).\n   -v : Arrêter la compilation après la phase de vérifications (défaut = non).\n   -h : Afficher la liste des options.\n\n  Les arguments -s et -v ne sont pas compatibles\n\n\n*******************************************************************************************************************\n\n");
        exit(0);
    }

    if(cflag >= 2){
        printf("Erreur : Il faut un seul fichier source en argument\n");
        exit(1);
    }

    if( !cflag^hbflag){
        printf("Erreur : Fichier source manquant ou mauvaise option spécifiée.\nUtilisez l'option -h pour avoir la liste des options.\n");
        exit(1);
    }
    cflag=0;
    // Fin verif
    // On parcourt toutes les options
    while( c != EOF ){   
            switch(c){
                case 'b' :
                    if(argc == 2){
                        printf("******************************************************\n\nMiniC Compiler. Written by BENNIS Ismail & EL KHARROUBI Karim\n\n******************************************************\n");
                        exit(0);
                    } else {
                        printf("Erreur : L'argument -b ne fonctione avec aucun autre arugment.\n");
                        exit(1);
                    }
                    break;
                case 'o' :
                    outfile = strdup(optarg);
                    break;

                case 't' :

                    if(strstr(chiffreT, optarg) == NULL){
                        printf("Erreur : argument de -t NULL ou invalide\n");
                        exit(1);
                    }
                    if( atoi(optarg) <= 5 && atoi(optarg) >= 0 )
                        trace_level = atoi(optarg);
                    else{
                        printf("Valeur argument -t invalide\n");
                        exit(1);
                    }
                break;
                case 'r' :
                    //printf("Arg r %d : %s, %s \n", optind, argv[optind], optarg);
                    if(strstr(chiffreR, optarg) == NULL){
                        printf("Erreur : argument de -r NULL ou invalide\n");
                        exit(1);
                    }
                    if( atoi(optarg) <= 8 && atoi(optarg) >= 4 )
                        set_max_registers(atoi(optarg));
                    else{
                        printf("Valeur argument -r invalide\n");
                        exit(1);
                    }
                    
                    break;

                case 's' :
                    if(!stop_after_verif){
                        stop_after_syntax = 1;
                    } else {
                        printf("Erreur : les arugments -s et -v ne sont pas compatibles.\n");
                        exit(1);
                    }
                    break;

                case 'v' :
                    if(!stop_after_syntax){
                        stop_after_verif = 1;
                    } else {
                        printf("Erreur : les arugments -s et -v ne sont pas compatibles.\n");
                        exit(1);
                    }
                    break;

                case 'h' :
                if(argc==2){
                    printf("*******************************************************************************************************************\n\n  Usage: ./minicc <options> <infile>\n <infile>  : Input file to compile (mandatory)\n\n                  Liste des options\n\n   -b : Affiche une bannière indiquant le nom du compilateur et des membres du binôme.\n   -o <filename> : Définit le nom du fichier assembleur produit (défaut : out.s).\n   -t <int> : Définit le niveau de trace à utiliser entre 0 et 5 (0 = pas de trace; 5 = toutes les traces. defaut = 0).\n   -r <int> : Définit le nombre maximum de registres à utiliser, entre 4 et 8 (défaut : 8).\n   -s : Arrêter la compilation après l'analyse syntaxique (défaut = non).\n   -v : Arrêter la compilation après la phase de vérifications (défaut = non).\n   -h : Afficher la liste des options.\n\n  Les arguments -s et -v ne sont pas compatibles\n\n\n*******************************************************************************************************************\n\n");
                    exit(0);
                } else {
                    printf("Erreur : L'argument -h ne fonctione avec aucun autre arugment.\n"); 
                    exit(1);
                }

                default :
                    break;
            }

            if( cflag == 0 ){
                if( strstr(argv[optind], ".c") != NULL){
                    infile = strdup(argv[optind]);
                    cflag++;
                }
            }
            if(optind==argc){
                break;
            }
            c = getopt(argc, argv, optstring);
    }
    

}


//int nb_noeud = 0;
void free_nodes(node_t n) {
    // A implementer
    
    if(n == NULL) return;
    if(n->nops != 0){
        for(int i = 0 ; i < n->nops ; i++){
            free_nodes(n->opr[i]);
        }
            
    }
    //nb_noeud++;
    //printf("Node %d freed\n", nb_noeud);
    free(n);
    

}


char * strdup(char * s) {
    char * r = malloc(strlen(s) + 1);
    strcpy(r, s);
    return r;
}


static int32_t dump_tree2dot_rec(FILE * f, node_t n, int32_t node_num) {

    if (n == NULL) {
        fprintf(f, "    N%d [shape=record, label=\"{{NULL}}\"];\n", node_num);
        return node_num;
    }

    switch (n->nature) {
        case NODE_IDENT:
            {
                node_t decl_node = n->decl_node;
                fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Type: %s}|{<decl>Decl      |Ident: %s|Offset: %d}}\"];\n", node_num, node_nature2string(n->nature), node_type2string(n->type), n->ident, n->offset);
                if (decl_node != NULL && decl_node != n) {
                    fprintf(f, "    edge[tailclip=false];\n");
                    fprintf(f, "    \"N%d\":decl:c -> \"N%d\" [style=dashed]\n", node_num, decl_node->node_num);
                }
                break;
            }
        case NODE_INTVAL:
        case NODE_BOOLVAL:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Type: %s}|{Value: %" PRId64 "}}\"];\n", node_num, node_nature2string(n->nature), node_type2string(n->type), n->value);
            break;
        case NODE_STRINGVAL:
            {
                char str[32];
                int32_t i = 1;
                while (true) {
                    str[i - 1] = n->str[i];
                    i += 1;
                    if (n->str[i] == '"') {
                        str[i - 1] = '\0';
                        break;
                    }
                }
                fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Type: %s}|{val: %s}}\"];\n", node_num, node_nature2string(n->nature), node_type2string(n->type), str);
            }
            break;
        case NODE_TYPE:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Type: %s}}\"];\n", node_num, node_nature2string(n->nature), node_type2string(n->type));
            break;
        case NODE_LIST:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE LIST}}\"];\n", node_num);
            break;
        case NODE_PROGRAM:
        case NODE_BLOCK:
        case NODE_DECLS:
        case NODE_DECL:
        case NODE_IF:
        case NODE_WHILE:
        case NODE_FOR:
        case NODE_DOWHILE:
        case NODE_PRINT:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Nb. ops: %d}}\"];\n", node_num, node_nature2string(n->nature), n->nops);
            break;
        case NODE_FUNC:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Nb. ops: %d}|{offset: %d}}\"];\n", node_num, node_nature2string(n->nature), n->nops, n->offset);
            break;
        case NODE_PLUS:
        case NODE_MINUS:
        case NODE_MUL:
        case NODE_DIV:
        case NODE_MOD:
        case NODE_LT:
        case NODE_GT:
        case NODE_LE:
        case NODE_GE:
        case NODE_EQ:
        case NODE_NE:
        case NODE_AND:
        case NODE_OR:
        case NODE_BAND:
        case NODE_BOR:
        case NODE_BXOR:
        case NODE_SRA:
        case NODE_SRL:
        case NODE_SLL:
        case NODE_NOT:
        case NODE_BNOT:
        case NODE_UMINUS:
        case NODE_AFFECT:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Type: %s|Nb. ops: %d}}\"];\n", node_num, node_nature2string(n->nature), node_type2string(n->type), n->nops);
            break;
        default:
            printf("*** Error in %s: unknow nature : %s\n", __func__, node_nature2string(n->nature));
            assert(false);
    }

    n->node_num = node_num;

    int32_t curr_node_num = node_num + 1;
    for (int32_t i = 0; i < n->nops; i += 1) {
        int32_t new_node_num = dump_tree2dot_rec(f, n->opr[i], curr_node_num);

        fprintf(f, "    edge[tailclip=true];\n");
        fprintf(f, "    N%d -> N%d\n", node_num, curr_node_num);
        curr_node_num = new_node_num + 1;
    }

    return curr_node_num - 1;
}



static void dump_tree2dot(FILE * f, node_t root) {
    assert(root->nature == NODE_PROGRAM);

    int32_t curr_node_num = 1;
    dump_tree2dot_rec(f, root, curr_node_num);
}


void dump_tree(node_t prog_root, const char * dotname) {

    FILE * f;

    f = fopen(dotname, "w");
    fprintf(f, "digraph global_vars {\n");
    dump_tree2dot(f, prog_root);
    fprintf(f, "}");    
    fclose(f);
}


const char * node_type2string(node_type t) {
    switch (t) {
        case TYPE_NONE:
            return "TYPE NONE";
        case TYPE_INT:
            return "TYPE INT";
        case TYPE_BOOL:
            return "TYPE BOOL";
        case TYPE_VOID:
            return "TYPE VOID";
        case TYPE_STRING:
            return "TYPE STRING";
        default:
            assert(false);
    }
}


const char * node_nature2string(node_nature t) {
    switch (t) {
        case NONE:
            return "NONE";
        case NODE_PROGRAM:
            return "PROGRAM";
        case NODE_BLOCK:
            return "BLOCK";
        case NODE_LIST:
            return "LIST";
        case NODE_DECLS:
            return "DECLS";
        case NODE_DECL:
            return "DECL";
        case NODE_IDENT:
            return "IDENT";
        case NODE_TYPE:
            return "TYPE";
        case NODE_INTVAL:
            return "INTVAL";
        case NODE_BOOLVAL:
            return "BOOLVAL";
        case NODE_STRINGVAL:
            return "STRINGVAL";
        case NODE_FUNC:
            return "FUNC";
        case NODE_IF:
            return "IF";
        case NODE_WHILE:
            return "WHILE";
        case NODE_FOR:
            return "FOR";
        case NODE_DOWHILE:
            return "DOWHILE";
        case NODE_PLUS:
            return "PLUS";
        case NODE_MINUS:
            return "MINUS";
        case NODE_MUL:
            return "MUL";
        case NODE_DIV:
            return "DIV";
        case NODE_MOD:
            return "MOD";
        case NODE_LT:
            return "LT";
        case NODE_GT:
            return "GT";
        case NODE_LE:
            return "LE";
        case NODE_GE:
            return "GE";
        case NODE_EQ:
            return "EQ";
        case NODE_NE:
            return "NE";
        case NODE_AND:
            return "AND";
        case NODE_OR:
            return "OR";
        case NODE_BAND:
            return "BAND";
        case NODE_BOR:
            return "BOR";
        case NODE_BXOR:
            return "BXOR";
        case NODE_SLL:
            return "SLL";
        case NODE_SRA:
            return "SRA";
        case NODE_SRL:
            return "SRL";
        case NODE_NOT:
            return "NOT";
        case NODE_BNOT:
            return "BNOT";
        case NODE_UMINUS:
            return "UMINUS";
        case NODE_AFFECT:
            return "AFFECT";
        case NODE_PRINT:
            return "PRINT";
        default:
            fprintf(stderr, "*** Error in %s: Unknown node nature: %d\n", __func__, t);
            exit(1);
    }
}




const char * node_nature2symb(node_nature t) {
    switch (t) {
        case NONE:
        case NODE_PLUS:
            return "+";
        case NODE_MINUS:
            return "-";
        case NODE_MUL:
            return "*";
        case NODE_DIV:
            return "/";
        case NODE_MOD:
            return "%";
        case NODE_LT:
            return "<";
        case NODE_GT:
            return ">";
        case NODE_LE:
            return "<=";
        case NODE_GE:
            return ">=";
        case NODE_EQ:
            return "==";
        case NODE_NE:
            return "!=";
        case NODE_AND:
            return "&&";
        case NODE_OR:
            return "||";
        case NODE_BAND:
            return "&";
        case NODE_BOR:
            return "|";
        case NODE_BXOR:
            return "^";
        case NODE_SRA:
            return ">>";
        case NODE_SRL:
            return ">>>";
        case NODE_SLL:
            return "<<";
        case NODE_NOT:
            return "!";
        case NODE_BNOT:
            return "~";
        case NODE_UMINUS:
            return "-";
        default:
            fprintf(stderr, "*** Error in %s: Unknown node nature: %d\n", __func__, t);
            exit(1);
    }
}




