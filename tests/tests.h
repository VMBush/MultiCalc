#ifndef _UNI_TEST_H
#define _UNI_TEST_H
#include <check.h>

#include "../qt_calc/c_funcs/calculations.h"

Suite* make_solve_polish_suite(void);
Suite* make_helpers_suite(void);
Suite* make_validators_suite(void);

#endif