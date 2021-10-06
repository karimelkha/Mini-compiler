
#include <stdio.h>
#include "./utils/miniccutils.h"
#include "defs.h"
#include "passe_2.h"
#include "common.h"

int32_t offset_temp;
int32_t strings_number;
char* temp_string;
bool first_reg = true;
bool temporary = false;
int32_t num_label = 1;
void boucle_print(node_t root);

void is_reg_8(){

	if(get_current_reg()==8){
		first_reg = true;
	}
	else if(temporary){
		temporary = false;
		pop_temporary(get_current_reg());
	}
	else{
		release_reg();
	}

}

void boucle_gen_passe_2(node_t root){

	if(root->nops!=0){
		for (int i = 0; i < root->nops; ++i)
			{
				if(root->opr[i]!=NULL){
					gen_code_passe_2(root->opr[i]);
				}
				
			}
	}
	
}

void switch_op(node_t root, int32_t reg_1, int32_t reg_2){

	switch(root->nature){

		case NODE_PLUS : 
			create_inst_addu(reg_1,reg_1,reg_2);
			break;

		case NODE_MINUS : 
			create_inst_subu(reg_1,reg_1,reg_2);
			break;

		case NODE_MUL : 
			create_inst_mult(reg_1,reg_2);
			break;

		case NODE_DIV : 
			create_inst_div(reg_1,reg_2);
			create_inst_teq(reg_2,0);
			create_inst_mflo(reg_1);
			break;

		case NODE_MOD : 
			create_inst_div(reg_1,reg_2);
			create_inst_teq(reg_2,0);
			create_inst_mfhi(reg_1);
			break;

		case NODE_BAND : 
			create_inst_and(reg_1,reg_1,reg_2);
			break;

		case NODE_BOR :
			create_inst_or(reg_1,reg_1,reg_2); 
			break;

		case NODE_EQ : 
			create_inst_xor(reg_1,reg_1,reg_2);
			is_reg_8();
			create_inst_sltiu(reg_1,reg_1,1);
			break;

		case NODE_NE : 
			create_inst_xor(reg_1,reg_1,reg_2);
			is_reg_8();
			create_inst_sltu(reg_1,0,reg_1);
			break;

		case NODE_LT : 
			create_inst_slt(reg_1,reg_1,reg_2);
			is_reg_8();
			break;

		case NODE_GT : 
			create_inst_slt(reg_1,reg_2,reg_1);
			is_reg_8();
			break;

		case NODE_LE : 
			create_inst_slt(reg_1,reg_2,reg_1);
			is_reg_8();
			create_inst_xori(reg_1,reg_1,1);
			break;

		case NODE_GE : 
			create_inst_slt(reg_1,reg_1,reg_2);
			is_reg_8();
			create_inst_xori(reg_1,reg_1,1);
			break;

		case NODE_AND : 
			create_inst_and(reg_1,reg_1,reg_2);
			is_reg_8();
			break;

		case NODE_OR : 
			create_inst_or(reg_1,reg_1,reg_2);
			is_reg_8();
			break;
	}

}

void explo_print(node_t root){

	switch(root->nature){

		case NODE_LIST : 
			boucle_print(root);
			break;

		case NODE_STRINGVAL : 
			create_inst_lui(4,4097);
			create_inst_ori(4,4,root->offset);
			create_inst_ori(2,0,4);
			create_inst_syscall();
			break;

		case NODE_IDENT : 
			create_inst_lw(4,root->offset,29);
			create_inst_ori(2,0,1);
			create_inst_syscall();
			break;

		default : 
			break;
	}
	
}

void boucle_print(node_t root){

	if(root->nops!=0){
		for (int i = 0; i < root->nops; ++i)
			{
				if(root->opr[i]!=NULL){
					explo_print(root->opr[i]);
				}
				
			}
	}
	
}

void node_decl(node_t root){

	int32_t temp_offset_init;
	int32_t temp_offset_decl;
	int32_t temp_reg;
	int32_t temp_offset;
	int32_t temp_value;
	node_t fils1 = root->opr[0];
	node_t fils2 = root->opr[1];

	if(fils1!=NULL&&fils2!=NULL){
		if(fils1->nature==NODE_IDENT && fils2->nature==NODE_IDENT){
			if(fils1->decl_node==NULL){
				temp_offset_init = fils1->offset;
				temp_offset_decl = fils2->offset;
			}else{
				temp_offset_init = fils2->offset;
				temp_offset_decl = fils1->offset;

			}
			temp_reg = reg();
			create_inst_lui(temp_reg,4097);
			create_inst_lw(temp_reg,temp_offset_decl,temp_reg);
			create_inst_sw(temp_reg,temp_offset_init,29);
			is_reg_8();
		}
		else if((fils1->nature==NODE_IDENT && (fils2->nature==NODE_INTVAL||fils2->nature==NODE_BOOLVAL))||((fils1->nature==NODE_INTVAL||fils1->nature==NODE_BOOLVAL) && fils2->nature==NODE_IDENT)){
			if(fils1->nature==NODE_IDENT){
				temp_offset = fils1->offset;
				temp_value = fils2->value;
			}else{
				temp_offset = fils2->offset;
				temp_value = fils1->value;
			}
			temp_reg = reg();
			create_inst_ori(temp_reg,0,temp_value);
			create_inst_sw(temp_reg,temp_offset,29);
			is_reg_8();
		}
	}
	
}




void node_for(node_t root){

	node_t affect_ident, affect_op, op, block;
	int32_t reg_1,reg_2;
	for (int i = 0; i < root->nops; ++i)
	{
		if(root->opr[i]->nature==NODE_AFFECT){
			if(root->opr[i]->opr[0]->nature==NODE_IDENT&&root->opr[i]->opr[1]->nature==NODE_IDENT){
				affect_ident = root->opr[i];
			}else{
				affect_op = root->opr[i];
			}
		}
		else if(root->opr[i]->nature==NODE_BLOCK){
			block = root->opr[i];
		}else{
			op = root->opr[i];
		}
	}
	reg_1 = reg();
	create_inst_lw(reg_1,affect_ident->opr[1]->offset,29);
	create_inst_sw(reg_1,affect_ident->opr[0]->offset,29);
	create_inst_label(num_label);
	create_inst_lw(reg_1,op->opr[0]->offset,29);
	reg_2 = reg();
	create_inst_lw(reg_2,op->opr[1]->offset,29);

	switch_op(root,reg_1,reg_2);
	create_inst_beq(reg_1,0,num_label+1);
	is_reg_8();
	boucle_gen_passe_2(block);
	gen_code_passe_2(affect_op);
	create_inst_j(num_label);
	num_label+=1;
	create_inst_label(num_label);
	num_label+=1;

}



int32_t reg_op(node_t root){

	int32_t reg_return,reg_1,reg_2;
	node_t fils1,fils2;
	fils1 = root->opr[0];
	fils2 = root->opr[1];
	reg_1 = reg();
	reg_2 = reg();
	
	if((fils1->nature==NODE_IDENT&&fils2->nature==NODE_IDENT)||(fils1->nature==NODE_IDENT&&fils2->nature==NODE_INTVAL)||(fils1->nature==NODE_INTVAL&&fils2->nature==NODE_IDENT)||(fils1->nature==NODE_INTVAL&&fils2->nature==NODE_INTVAL)){
		if(fils1->nature==NODE_IDENT&&fils2->nature==NODE_IDENT){
			if(fils1->global_decl==true){
				create_inst_lui(reg_1,4097);
			}
			create_inst_lw(reg_1,fils1->offset,29);
			if(fils2->global_decl==true){
				create_inst_lui(reg_2,4097);
			}
			create_inst_lw(reg_2,fils2->offset,29);
			switch_op(root,reg_1,reg_2);
			is_reg_8();
			reg_return = reg_1;
		}
		else if(fils1->nature==NODE_IDENT&&fils2->nature==NODE_INTVAL){
			if(fils1->global_decl==true){
				create_inst_lui(reg_1,4097);
			}
			create_inst_lw(reg_1,fils1->offset,29);
			create_inst_ori(reg_2,0,fils2->value);
			switch_op(root,reg_1,reg_2);
			is_reg_8();
			reg_return = reg_1;
		}
		else if(fils2->nature==NODE_IDENT&&fils1->nature==NODE_INTVAL){
			if(fils2->global_decl==true){
				create_inst_lui(reg_2,4097);
			}
			create_inst_lw(reg_2,fils2->offset,29);
			create_inst_ori(reg_1,0,fils1->value);
			switch_op(root,reg_1,reg_2);
			is_reg_8();
			reg_return = reg_1;
		}
		else if(fils1->nature==NODE_INTVAL&&fils2->nature==NODE_INTVAL){
			create_inst_ori(reg_1,0,fils1->value);
			create_inst_ori(reg_2,0,fils2->value);
			switch_op(root,reg_1,reg_2);
			is_reg_8();
			reg_return = reg_1;
		}
	}else{
		if(fils1->nature==NODE_IDENT||fils1->nature==NODE_INTVAL){
			if(fils1->nature==NODE_IDENT){
				if(fils1->global_decl==true){
					create_inst_lui(reg_1,4097);
				}
				create_inst_lw(reg_1,fils1->offset,29);
			}else{
				create_inst_ori(reg_1,0,fils1->value);
			}
			release_reg();
			reg_2 = reg_op(fils2);
			switch_op(root,reg_1,reg_2);
			is_reg_8();
			reg_return = reg_1;
		}else{ // fils2 ident out intval
			release_reg();
			reg_1 = reg_op(fils1);
			if(fils2->nature==NODE_IDENT){
				if(fils2->global_decl==true){
					create_inst_lui(reg_2,4097);
				}else{
					create_inst_ori(reg_2,0,fils2->value);
				}
				create_inst_lw(reg_2,fils2->offset,29);
			}
			switch_op(root,reg_1,reg_2);
			is_reg_8();
			reg_return = reg_1;
		}
	}
	return reg_return;

}


int32_t reg(){

	int32_t num_reg;
	int32_t reg_restore;
	if(reg_available()==true){
		if(get_current_reg()==8 && first_reg==true){
			first_reg=false;
			num_reg = get_current_reg();
		}else{
			allocate_reg();
			num_reg = get_current_reg();
		}
		
	}else{
		temporary = true;
		reg_restore = get_restore_reg();
		push_temporary(reg_restore);
		num_reg = get_current_reg();
	}
	return num_reg;

}

void node_if(node_t root){

	node_t block1,block2,op;
	int32_t r_op;

	if(root->nops==2){
		// c un if
		op = root->opr[0];
		block1 = root->opr[1];

		r_op = reg_op(op);
		create_inst_beq(r_op,0,num_label);
		is_reg_8();
		gen_code_passe_2(block1);
		create_inst_label(num_label);
		num_label++;

	}else{
		// c un if/else
		op = root->opr[0];
		block1 = root->opr[1];
		block2 = root->opr[2];
		r_op = reg_op(op);
		create_inst_beq(r_op,0,num_label);
		is_reg_8();
		gen_code_passe_2(block1);
		create_inst_j(num_label+1);
		create_inst_label(num_label);
		gen_code_passe_2(block2);
		create_inst_label(num_label+1);
		num_label+=2;
	}

}

void node_while(node_t root){

	node_t op,block;
	op = root->opr[0];
	block = root->opr[1];
	int32_t r_op;
	create_inst_label(num_label);
	r_op = reg_op(op);
	create_inst_beq(r_op,0,num_label+1);
	is_reg_8();
	gen_code_passe_2(block);
	create_inst_j(num_label);
	create_inst_label(num_label+1);
	num_label+=2;
}

void node_dowhile(node_t root){

	node_t op,block;
	block = root->opr[0];
	op = root->opr[1];
	int32_t r_op;
	create_inst_label(num_label);
	gen_code_passe_2(block);
	r_op = reg_op(op);
	create_inst_bne(r_op,0,num_label);
	num_label++;

}


void gen_code_passe_2(node_t root) {

	char* temp_label;
	int32_t temp_value;
	bool is_glob_decl = false;
	bool is_val = false;
	int32_t reg_1,reg_2;
	node_t fils1,fils2;
	switch(root->nature){

		case NODE_PROGRAM : 
			
			if(root->opr[0]->nature!=NODE_FUNC){
				// si node_list
				create_inst_data_sec();
				if(root->opr[0]!=NULL){
					gen_code_passe_2(root->opr[0]);	
				}
				strings_number = get_global_strings_number();
				if(strings_number!=0){
					for (int i = 0; i < strings_number; ++i)
					{
						temp_string = get_global_string(i);
						create_inst_asciiz(NULL,temp_string);
					}	
				}
				create_inst_text_sec();
				create_inst_label_str("main");
				gen_code_passe_2(root->opr[1]);
			}else{
				create_inst_data_sec();
				strings_number = get_global_strings_number();
				if(strings_number!=0){
					for (int i = 0; i < strings_number; ++i)
					{
						temp_string = get_global_string(i);
						create_inst_asciiz(NULL,temp_string);
					}	
				}
				create_inst_text_sec();
				create_inst_label_str("main");
				gen_code_passe_2(root->opr[0]);

			}
			
			create_inst_ori(2,0,10);
			create_inst_syscall();
			break;

		case NODE_FUNC : 
			reset_temporary_max_offset();
			create_inst_stack_allocation();
			set_temporary_start_offset(root->offset);
			boucle_gen_passe_2(root);
			offset_temp = get_temporary_max_offset();
			create_inst_stack_deallocation(root->offset+offset_temp);
			break;

		case NODE_LIST : 
			boucle_gen_passe_2(root);
			break;
		case NODE_BLOCK : 
			boucle_gen_passe_2(root);
			break;
		case NODE_DECLS : 
			boucle_gen_passe_2(root);
			break;
		case NODE_IDENT : 
			boucle_gen_passe_2(root);
			break;

		case NODE_DECL : 
			
			for (int i = 0; i < root->nops; ++i){
				
				if(root->opr[i]!=NULL){
					if(root->opr[i]->nature==NODE_IDENT && root->opr[i]->global_decl==true){
						temp_label = root->opr[i]->ident;
						is_glob_decl = true;
					}
					else if(root->opr[i]->nature==NODE_INTVAL||root->opr[i]->nature==NODE_BOOLVAL){
						temp_value = root->opr[i]->value;
					}
				}
			}
			if(is_glob_decl==true){
				is_glob_decl=false;
				create_inst_word(temp_label,temp_value);
			}else{
				node_decl(root);
			}
			break;

		case NODE_FOR : 
			node_for(root);
			break;
		case NODE_PRINT : 
			boucle_print(root);
			break;

		case NODE_AFFECT : 
			
			if(root->opr[1]->nature==NODE_IDENT){
				reg_1 = reg();
				if(root->opr[1]->global_decl==true){
					create_inst_lui(reg_1,4097);
				}
				create_inst_lw(reg_1,root->opr[1]->offset,reg_1);
				create_inst_sw(reg_1,root->opr[0]->offset,29);		
			}
			else if(root->opr[1]->nature==NODE_INTVAL||root->opr[1]->nature==NODE_BOOLVAL){
				create_inst_ori(reg_1,0,root->opr[1]->value);
				create_inst_sw(reg_1,root->opr[0]->offset,29);
			}
			else{
				reg_2 = reg_op(root->opr[1]);
				create_inst_sw(reg_2,root->opr[0]->offset,29);

			}
			is_reg_8();
			break;

		case NODE_IF : 
			node_if(root);
			break;

		case NODE_PLUS : 
			break;
		case NODE_MINUS : 
			break;
		case NODE_MUL : 
			break;
		case NODE_DIV : 
			break;
		case NODE_MOD : 
			break;
		case NODE_LT : 
			break;
		case NODE_GT : 
			break;
		case NODE_LE : 
			break;
		case NODE_GE : 
			break;
		case NODE_EQ : 
			break;
		case NODE_AND : 
			break;
		case NODE_OR : 
			break;
		case NODE_BAND : 
			break;
		case NODE_BOR : 
			break;
		case NODE_BXOR : 
			break;
		case NODE_NOT : 
			break;
		case NODE_BNOT : 
			break;
		case NODE_SLL : 
			break;
		case NODE_SRA : 
			break;
		case NODE_SRL : 
			break;
		case NODE_UMINUS : 
			break;
		case NODE_BOOLVAL : 
			break;
		case NODE_STRINGVAL : 
			break;
		case NODE_INTVAL : 
			break;
		case NODE_WHILE : 
			node_while(root);
			break;
		case NODE_DOWHILE :
			node_dowhile(root);
			break;
		case NODE_TYPE : 
			break;
		case NONE : 
			break;
		default : 
			break;
	}
	
}

