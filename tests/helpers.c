#include "tests.h"
#define CK_FORK no

START_TEST(test_1) {
  ck_assert_int_eq(openned_brackets_count("(3+x)"), 0);
  ck_assert_int_eq(openned_brackets_count("sin(cos(3+x)"), 1);
}
END_TEST

START_TEST(test_2) {
  char* str = malloc(100 * sizeof(char));
  strcpy(str, "(2+2,0)*2+0,5");
  delete_last_symbol(str);
  ck_assert_str_eq(str, "(2+2,0)*2+0,");

  strcpy(str, "6");
  delete_last_symbol(str);
  ck_assert_str_eq(str, "0");


  strcpy(str, "sin(");
  delete_last_symbol(str);
  ck_assert_str_eq(str, "0");

  strcpy(str, "3+(");
  delete_last_symbol(str);
  ck_assert_str_eq(str, "3+");

  strcpy(str, "3+ln(");
  delete_last_symbol(str);
  ck_assert_str_eq(str, "3+");

  strcpy(str, "3+asin(");
  delete_last_symbol(str);
  ck_assert_str_eq(str, "3+");

  strcpy(str, "3+acos(");
  delete_last_symbol(str);
  ck_assert_str_eq(str, "3+");

  strcpy(str, "3+atan(");
  delete_last_symbol(str);
  ck_assert_str_eq(str, "3+");

  strcpy(str, "3+log(");
  delete_last_symbol(str);
  ck_assert_str_eq(str, "3+");

  strcpy(str, "3+cos(");
  delete_last_symbol(str);
  ck_assert_str_eq(str, "3+");

  strcpy(str, "3+tan(");
  delete_last_symbol(str);
  ck_assert_str_eq(str, "3+");

  free(str);
}
END_TEST


Suite* make_helpers_suite(void) {
  Suite* s = suite_create("helpers");
  TCase* tc = tcase_create("case");

  tcase_add_test(tc, test_1);
  tcase_add_test(tc, test_2);

  suite_add_tcase(s, tc);
  return s;
}
