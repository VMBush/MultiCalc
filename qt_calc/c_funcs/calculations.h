#include "../smartcalc.h"
#ifndef POLISH_H
#define POLISH_H



typedef enum {
	NUMBER,
	FUNC,
	OPEN_BR,
	CLOSE_BR,
	OPERATOR,
} lexemm_types;

typedef struct 
{
	lexemm_types type;
	long double number;
	char func[10];
	char operator;
	int operator_priority;
/* data */
} lexemm;

//struct lexemm_list;

typedef struct l_lst
{
	lexemm lex;
	struct l_lst* prev;
	struct l_lst* next;
} lexemm_list;

lexemm_list* to_polish(char* src);
lexemm_list* to_lexemm_list(char* src);
char* solve_polish(lexemm_list* lexemms);
void free_lexemm_list(lexemm_list* list);
void scan_lexemm(char* src, lexemm* lex, int* sym_num);
lexemm_list* add_missing_operators(lexemm_list* item);
lexemm_list* push_lexemm_stacked(lexemm lex, lexemm_list* list);
lexemm pop_lexemm_stacked(lexemm_list** list);
lexemm pop_n_free_lexemm(lexemm_list* el);

#endif