#ifndef SMARTCALC_H
#define SMARTCALC_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>



#ifdef __cplusplus
    extern "C" {
#endif
    
int insertion_validator(char* dest, char* el);
int insertion_num_validator(char* dest, char* el);
int calculate_validator(char* dest);
int graphic_validator(char* dest);

int isnum(char el);
int openned_brackets_count(char* str);
void delete_last_symbol(char* str);

char* solve_expression(char* str);


#ifdef __cplusplus
    }
#endif

#endif  //SMARTCALC_H
