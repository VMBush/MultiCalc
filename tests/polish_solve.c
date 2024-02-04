#include "tests.h"
#define CK_FORK no

START_TEST(test_1) {
  char* str = malloc(100 * sizeof(char));
  strcpy(str, "(2+2,0)*2+0,5");
  char* res = solve_expression(str);
  ck_assert_str_eq(res, "8,5");
  free(str);
}
END_TEST

START_TEST(test_2) {
  char* str = malloc(100 * sizeof(char));
  strcpy(str, "2+2*2");
  char* res = solve_expression(str);
  ck_assert_str_eq(res, "6");
  free(str);
}
END_TEST

START_TEST(test_3) {
  char* str = malloc(100 * sizeof(char));
  strcpy(str, "2*2*2");
  char* res = solve_expression(str);
  ck_assert_str_eq(res, "8");
  free(str);
}
END_TEST

START_TEST(test_4) {
  char* str = malloc(100 * sizeof(char));
  strcpy(str, "ln(log(sin(asin(cos(acos(tan(atan(25%100/1))))))))");
  char* res = solve_expression(str);
  ck_assert_str_eq(res, "ERROR");
  free(str);
}
END_TEST

START_TEST(test_5) {
  char* str = malloc(100 * sizeof(char));
  strcpy(str, "-(-5^2)(v4)");
  char* res = solve_expression(str);
  ck_assert_str_eq(res, "50");
  free(str);
}
END_TEST

START_TEST(test_6) {
  char* str = malloc(100 * sizeof(char));
  strcpy(str, "999999999^999999999");
  char* res = solve_expression(str);
  ck_assert_str_eq(res, "inf");
  free(str);
}
END_TEST

START_TEST(test_7) {
  char* str = malloc(100 * sizeof(char));
  strcpy(str, "-999999999^999999999");
  char* res = solve_expression(str);
  ck_assert_str_eq(res, "-inf");
  free(str);
}
END_TEST


Suite* make_solve_polish_suite(void) {
  Suite* s = suite_create("solve_polish");
  TCase* tc = tcase_create("case");

  tcase_add_test(tc, test_1);
  tcase_add_test(tc, test_2);
  tcase_add_test(tc, test_3);
  tcase_add_test(tc, test_4);
  tcase_add_test(tc, test_5);
  tcase_add_test(tc, test_6);
  tcase_add_test(tc, test_7);

  suite_add_tcase(s, tc);
  return s;
}
