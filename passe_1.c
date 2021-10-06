
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "defs.h"
#include "passe_1.h"
#include "./utils/miniccutils.h"
extern int trace_level;

bool new_context = true;
node_type last_node_type_node_decls;
node_type temp_node_type;
node_type verif_type_affect;
node_t condition_apres_contexte = NULL;
node_t temp_node_list = NULL;
node_t temp_node_op = NULL;
node_t temp_node_func = NULL;
int32_t temp_offset;
int32_t offset_global=0;
node_type type1,type2;
void explo_node_op(node_t root);
void explo_node_program(node_t root);
node_type type1,type2;

void boucle_analyse_passe_1(node_t root){

	if(root->nops!=0){
		for (int i = 0; i < root->nops; ++i)
			{
				if(root->opr[i]!=NULL){
					analyse_passe_1(root->opr[i]);
				}
			}
	}

}

void explo_node_affect(node_t root){

	if(root->opr[0]->nature==NODE_IDENT||root->opr[0]->nature==NODE_INTVAL){
				if(root->opr[0]->nature==NODE_IDENT){
					if(get_decl_node(root->opr[0]->ident)==NULL){
						fprintf(stderr, "Error line %d: variable '%s' undeclared.\n", root->lineno , root->opr[0]->ident);
						exit(1);
					}
				}
				analyse_passe_1(root->opr[0]);
				type1 = root->opr[0]->type;
	}else{
		if(root->opr[0]->nature == NODE_BNOT || root->opr[0]->nature == NODE_NOT || root->opr[0]->nature == NODE_UMINUS){
			explo_node_op(root->opr[0]);	
		}
		else{
			explo_node_op(root->opr[0]);
			type1 = root->opr[0]->type;	
		}
	}
	if(root->opr[1]->nature==NODE_IDENT||root->opr[1]->nature==NODE_INTVAL){
		if(root->opr[1]->nature==NODE_IDENT){
					if(get_decl_node(root->opr[1]->ident)==NULL){
						fprintf(stderr, "Error line %d: variable '%s' undeclared.\n", root->lineno , root->opr[1]->ident);
						exit(1);
					}
		}
		analyse_passe_1(root->opr[1]);
		type2 = root->opr[1]->type;
	}else{
		if(root->opr[1]->nature == NODE_BNOT || root->opr[1]->nature == NODE_NOT || root->opr[1]->nature == NODE_UMINUS){
			explo_node_op(root->opr[1]);	
		}
		else{
			explo_node_op(root->opr[1]);
			type2 = root->opr[1]->type;	
		}
	}
	if(type1==TYPE_INT && type2==TYPE_INT){
		root->type = TYPE_INT;	
	}
	else if(type1==TYPE_BOOL && type2==TYPE_BOOL){
		root->type = TYPE_BOOL;
	}
	else{
		fprintf(stderr, "Error line %d: operator '%s'cannot have operand of type %s.\n", root->lineno, node_type2string(type1), node_type2string(type2));
		exit(1);
	}		

}

void type_op_binaire_int_int_bool(node_t root){

	if(root->opr[0]->nature==NODE_IDENT||root->opr[0]->nature==NODE_INTVAL){
		analyse_passe_1(root->opr[0]);
		type1 = root->opr[0]->type;
	}else{
		explo_node_op(root->opr[0]);
		type1 = root->opr[0]->type;
	}
	if(root->opr[1]->nature==NODE_IDENT||root->opr[1]->nature==NODE_INTVAL){
		analyse_passe_1(root->opr[1]);
		type2 = root->opr[1]->type;
	}else{
		explo_node_op(root->opr[1]);
		type2 = root->opr[1]->type;
	}
	if(type1==TYPE_INT && type2==TYPE_INT){
		root->type = TYPE_BOOL;
	}else{
		printf("Error line %d: Les operandes n'ont pas les memes types\n", root->lineno);
		exit(1);
	}

}

void type_op_binaire_bool_bool_bool(node_t root){

	if(root->opr[0]->nature==NODE_IDENT||root->opr[0]->nature==NODE_BOOLVAL){
		analyse_passe_1(root->opr[0]);
		type1 = root->opr[0]->type;
	}else{
		explo_node_op(root->opr[0]);
		type1 = root->opr[0]->type;
	}
	if(root->opr[1]->nature==NODE_IDENT||root->opr[1]->nature==NODE_INTVAL){
		analyse_passe_1(root->opr[1]);
		type2 = root->opr[1]->type;
	}else{
		explo_node_op(root->opr[1]);
		type2 = root->opr[1]->type;
	}
	if(type1==TYPE_INT && type2==TYPE_INT){
		root->type = TYPE_BOOL;
	}else{
		printf("Error line %d:Les operandes n'ont pas les memes types", root->lineno);
		exit(1);
	}

}

void type_op_binaire_int_int_int(node_t root){

	if(root->opr[0]->nature==NODE_IDENT||root->opr[0]->nature==NODE_INTVAL){
		analyse_passe_1(root->opr[0]);
		type1 = root->opr[0]->type;
	}else{
		explo_node_op(root->opr[0]);
		type1 = root->opr[0]->type;
	}
	if(root->opr[1]->nature==NODE_IDENT||root->opr[1]->nature==NODE_INTVAL){
		analyse_passe_1(root->opr[1]);
		type2 = root->opr[1]->type;
	}else{
		explo_node_op(root->opr[1]);
		type2 = root->opr[1]->type;
	}
	if(type1==TYPE_INT && type2==TYPE_INT){
		root->type = type1;
	}else{

		if(type2 != TYPE_INT || type2 == TYPE_BOOL)
			printf("Error line %d: operator '%s' cannot have right operand of type %s.\n", root->lineno, node_nature2symb(root->nature), node_type2string(type2));
		else
			printf("Error line %d: operator '%s' cannot have left operand of type %s.\n", root->lineno, node_nature2symb(root->nature), node_type2string(type1));

		exit(1);
	}

}

void type_op_un(node_t root){

	if(root->opr[0]->nature==NODE_IDENT||root->opr[0]->nature==NODE_INTVAL||root->opr[0]->nature==NODE_BOOLVAL){
		analyse_passe_1(root->opr[0]);
		type1 = root->opr[0]->type;
	}else{
		explo_node_op(root->opr[0]);
		type1 = root->opr[0]->type;
	}
	if(type1==TYPE_INT && root->nature==NODE_UMINUS){
		root->type = type1;
	}
	else if(type1==TYPE_INT && root->nature==NODE_BNOT){
		root->type = type1;
	}
	else if(type1==TYPE_BOOL && root->nature==NODE_NOT){
		root->type = type1;
	} else if(type1==TYPE_BOOL && root->nature == NODE_BNOT){
		printf("Error line %d: operator '%s' cannot have operand of type %s", root->lineno, node_nature2symb(root->nature), node_type2string(root->type));
		exit(1);
	}else{
		printf("Error line %d: Les operandes n'ont pas les memes types 3\n", root->lineno);
		exit(1);
	}

}


void explo_node_op(node_t root){

	switch(root->nature){
		case NODE_AFFECT : 
			explo_node_affect(root);	
			break;
		case NODE_PLUS :
			type_op_binaire_int_int_int(root);
			break;
		case NODE_MINUS : 
			type_op_binaire_int_int_int(root);
			break;
		case NODE_MUL : 
			type_op_binaire_int_int_int(root);
			break;
		case NODE_DIV :
			type_op_binaire_int_int_int(root);
			break;
		case NODE_MOD : 
			type_op_binaire_int_int_int(root);
			break;
		case NODE_BAND : 
			type_op_binaire_int_int_int(root);
			break;
		case NODE_BOR :
			type_op_binaire_int_int_int(root);
			break;
		case NODE_BXOR : 
			type_op_binaire_int_int_int(root);
			break;
		case NODE_SLL : 
			type_op_binaire_int_int_int(root);
			break;
		case NODE_SRL :
			type_op_binaire_int_int_int(root);
			break;
		case NODE_SRA : 
			type_op_binaire_int_int_int(root);
			break;

		case NODE_EQ : 
			boucle_analyse_passe_1(root);
			if(root->opr[0]->nature!=NODE_IDENT&&root->opr[0]->nature!=NODE_INTVAL&&root->opr[0]->nature!=NODE_BOOLVAL){
				if(root->opr[1]->nature!=NODE_IDENT&&root->opr[1]->nature!=NODE_INTVAL&&root->opr[1]->nature!=NODE_BOOLVAL){
					explo_node_op(root->opr[0]);
					explo_node_op(root->opr[1]);
					if(root->opr[0]->type==TYPE_BOOL||root->opr[1]->type==TYPE_BOOL){
						type_op_binaire_bool_bool_bool(root);
					}else{
						type_op_binaire_int_int_bool(root);
					}
				}
			}else{
				if(root->opr[0]->type==TYPE_BOOL||root->opr[1]->type==TYPE_BOOL){
					type_op_binaire_bool_bool_bool(root);
				}else{
					type_op_binaire_int_int_bool(root);
				}
			}
			break;

		case NODE_NE : 
			boucle_analyse_passe_1(root);
			if(root->opr[0]->nature!=NODE_IDENT&&root->opr[0]->nature!=NODE_INTVAL&&root->opr[0]->nature!=NODE_BOOLVAL){
				if(root->opr[1]->nature!=NODE_IDENT&&root->opr[1]->nature!=NODE_INTVAL&&root->opr[1]->nature!=NODE_BOOLVAL){
					explo_node_op(root->opr[0]);
					explo_node_op(root->opr[1]);
					if(root->opr[0]->type==TYPE_BOOL||root->opr[1]->type==TYPE_BOOL){
						type_op_binaire_bool_bool_bool(root);
					}else{
						type_op_binaire_int_int_bool(root);
					}
				}
			}else{
				if(root->opr[0]->type==TYPE_BOOL||root->opr[1]->type==TYPE_BOOL){
					type_op_binaire_bool_bool_bool(root);
				}else{
					type_op_binaire_int_int_bool(root);
				}
			}
			break;

		case NODE_LT : 
			type_op_binaire_int_int_bool(root);
			break;
		case NODE_GT : 
			type_op_binaire_int_int_bool(root);
			break;
		case NODE_LE : 
			type_op_binaire_int_int_bool(root);
			break;
		case NODE_GE : 
			type_op_binaire_int_int_bool(root);
			break;
		case NODE_AND : 
			type_op_binaire_bool_bool_bool(root);
			break;
		case NODE_OR : 
			type_op_binaire_bool_bool_bool(root);
			break;

		default : 
			break;

	}
}


void boucle_explo_node_program(node_t root){

	if(root->nops!=0){
		for (int i = 0; i < root->nops; ++i)
			{
				if(root->opr[i]!=NULL){
					explo_node_program(root->opr[i]);
				}
				
			}
	}
	
}

void explo_node_program(node_t root){

	node_t ident,val;
	switch(root->nature){

		case NODE_FUNC : 
		    temp_node_func = root;
			break;
		case NODE_LIST : 
			boucle_explo_node_program(root);
			break;
		case NODE_DECLS : 
			boucle_explo_node_program(root);
			break;
		case NODE_TYPE : 
			temp_node_type = root->type;
			break;

		case NODE_DECL : 
			ident = root->opr[0];
			val = root->opr[1];
			boucle_explo_node_program(root);
			if(val != NULL && ident != NULL){
				if(ident->type != val->type){
					printf("Error line %d: variable of type %s is initialized with an expression of type %s.\n", root->lineno, node_type2string(ident->type),node_type2string(val->type));
					exit(1);
				}
			}
			break;

		case NODE_IDENT : 
			root->global_decl = true;
			root->type = temp_node_type;
			if(root->type==TYPE_VOID){
						printf("Error line %d: variable of type void.\n", root->lineno);
						exit(1);
			}
			temp_offset = env_add_element(root->ident,root);
			if(temp_offset<0){
				printf("Error line %d: variable %s defined multiple times.\n", root->lineno, root->ident);
				exit(1);
			}else{
				root->offset = temp_offset;
			}
			break;

		case NODE_INTVAL : 
			if(root->value < 0 || root->value > 4294967295){
				root->value = 0;
			}
			if(temp_node_type!=TYPE_INT){
				printf("Error line :variable globale avec type : %s initialisation pas avec bon type int\n",node_type2string(temp_node_type));
				exit(1);
			}
			break;

		case NODE_BOOLVAL : 
			if(root->value!=0 && root->value!=1){
				//printf("root->value : %ld\n",root->value);
				root->value = 0;
			}
			if(root->type!=TYPE_BOOL){
				printf("Error line :variable globale avec type : %s initialisation pas avec bon type bool\n",node_type2string(temp_node_type));
				exit(1);
			}
			break;

		default : 
			break;

	}
}


void analyse_passe_1(node_t root){

	node_t ident,val;
	switch(root->nature){

		case NODE_PROGRAM :

			temp_node_list = (node_t) malloc(sizeof(node_s));
			temp_node_func = (node_t) malloc(sizeof(node_s));

			push_global_context();
			boucle_explo_node_program(root);	
			analyse_passe_1(temp_node_func);
			pop_context();
			free(temp_node_list);
			free(temp_node_func);
			break;

		case NODE_FUNC : 

			reset_env_current_offset();
			// for if else if errror
			if(root->nops!=0){
				for (int i = 1; i < root->nops; ++i)
					{
						if(root->opr[i]!=NULL){
							analyse_passe_1(root->opr[i]);
					}
						
				}
			}
			for (int i = 0; i < root->nops; ++i)
			{
				if(root->opr[i]->nature==NODE_TYPE){
					if (root->opr[i]->type!=TYPE_VOID)
					{
						printf("Error line %d: Return type of 'main()' function should be 'void'.\n", root->lineno);
						exit(1);
					}
				}
				else if(root->opr[i]->nature==NODE_IDENT){
					//strcmp(str1,str2)
					if(strcmp(root->opr[i]->ident,"main")!=0) {
						printf("Error line %d: the name of the main function should be 'main'.\n", root->lineno);
						//printf("%s\n", root->opr[i]->ident);
						exit(1);
					}
				}
			}

			root->offset = get_env_current_offset();
			break;

		case NODE_BLOCK : 
			push_context();
			boucle_analyse_passe_1(root);
			pop_context();
			break;

		case NODE_IF : 	
			boucle_analyse_passe_1(root);	
			break;
		case NODE_FOR : 
			boucle_analyse_passe_1(root);
			break;
		case NODE_WHILE : 
			boucle_analyse_passe_1(root);
			break;
		case NODE_DOWHILE : 
			boucle_analyse_passe_1(root);
			break;
		case NODE_LT : 
			explo_node_op(root);
			boucle_analyse_passe_1(root);
			break;
		case NODE_GT : 
			explo_node_op(root);
			boucle_analyse_passe_1(root);
			break;
		case NODE_LE : 
			explo_node_op(root);
			boucle_analyse_passe_1(root);
			break;	
		case NODE_GE : 
			explo_node_op(root);
			boucle_analyse_passe_1(root);
			break;		
		case NODE_EQ : 
			explo_node_op(root);
			boucle_analyse_passe_1(root);
			break;	
		case NODE_NE : 
			explo_node_op(root);
			boucle_analyse_passe_1(root);
			break;	
		case NODE_AND : 
			explo_node_op(root);
			boucle_analyse_passe_1(root);
			break;	
		case NODE_OR : 
			explo_node_op(root);
			boucle_analyse_passe_1(root);
			break;	
		case NODE_BAND : 
			explo_node_op(root);
			boucle_analyse_passe_1(root);
			break;	
		case NODE_BOR : 
			explo_node_op(root);
			boucle_analyse_passe_1(root);
			break;	
		case NODE_BXOR : 
			explo_node_op(root);
			boucle_analyse_passe_1(root);
			break;	
		case NODE_NOT : 
			explo_node_op(root);
			boucle_analyse_passe_1(root);
			break;	
		case NODE_BNOT : 
			explo_node_op(root);
			boucle_analyse_passe_1(root);
			break;	
		case NODE_SLL : 
			explo_node_op(root);
			boucle_analyse_passe_1(root);
			break;	
		case NODE_SRL : 
			explo_node_op(root);
			boucle_analyse_passe_1(root);
			break;	
		case NODE_SRA : 
			explo_node_op(root);
			boucle_analyse_passe_1(root);
			break;													
		case NODE_UMINUS : 
			boucle_analyse_passe_1(root);
			break;		
		case NODE_AFFECT : 
			explo_node_affect(root);
			break;
		case NODE_PLUS : 
			explo_node_op(root);
			boucle_analyse_passe_1(root);
			break;
		case NODE_MINUS : 
			explo_node_op(root);
			boucle_analyse_passe_1(root);
			break;
		case NODE_MUL : 
			explo_node_op(root);
			boucle_analyse_passe_1(root);
			break;
		case NODE_DIV : 
			explo_node_op(root);
			boucle_analyse_passe_1(root);
			break;		
		case NODE_MOD : 
			explo_node_op(root);
			boucle_analyse_passe_1(root);
			break;	
		case NODE_PRINT : 
			boucle_analyse_passe_1(root);
			break;
		case NODE_LIST : 
			boucle_analyse_passe_1(root);		
			break;
		case NODE_DECLS : 
			boucle_analyse_passe_1(root);
			break;

		case NODE_TYPE : 
			if(root->type!=TYPE_VOID){
				last_node_type_node_decls = root->type;
			}
			boucle_analyse_passe_1(root);
			break;

		case NODE_DECL : 

			ident = root->opr[0];
			val = root->opr[1];
			boucle_analyse_passe_1(root);

			if(val != NULL && ident != NULL){
				if(ident->type != val->type){
					printf("Error line %d: variable of type %s is initialized with an expression of type %s.\n", root->lineno, node_type2string(ident->type),node_type2string(val->type));
					exit(1);
				}
			}
			boucle_analyse_passe_1(root);
			break;

		case NODE_IDENT : 

			if(strcmp(root->ident,"main")!=0){
				// si node_ident pas deja present
				if(get_decl_node(root->ident)==NULL){

					root->global_decl = false;
					root->type = last_node_type_node_decls;
					if(root->type==TYPE_VOID){
						printf("Error line %d: variable of type void.\n", root->lineno);
						exit(1);
					}
					temp_offset = env_add_element(root->ident,root);
					if(temp_offset<0){
						printf("Error line %d: variable %s defined multiple times.\n", root->lineno, root->ident);
						exit(1);
					}
					else{
						root->offset = temp_offset;
					}
				}else{
					root->decl_node = get_decl_node(root->ident);
					root->offset = root->decl_node->offset;
					root->type = root->decl_node->type;
				}
				
				boucle_analyse_passe_1(root);
			}
			
			break;

		case NODE_INTVAL : 
			boucle_analyse_passe_1(root);
			break;
		case NODE_STRINGVAL : 
			root->offset = add_string(root->str); 
			boucle_analyse_passe_1(root);
			break;
		case NODE_BOOLVAL : 
			boucle_analyse_passe_1(root);
			break;
		case NONE : 
			boucle_analyse_passe_1(root);
			break;
		default : 
			break;

	}
}