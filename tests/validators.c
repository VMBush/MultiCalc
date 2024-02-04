#include "tests.h"
#define CK_FORK no

START_TEST(test_insert) {
  ck_assert_int_eq(insertion_validator("0", "+"), 1);
  ck_assert_int_eq(insertion_validator("0,", "x"), 0);
  ck_assert_int_eq(insertion_validator("x", "x"), 0);
  ck_assert_int_eq(insertion_validator("25", "("), 1);
  ck_assert_int_eq(insertion_validator("-4", "*"), 1);
  ck_assert_int_eq(insertion_validator("99(", "/"), 0);
  ck_assert_int_eq(insertion_validator("2", "-"), 1);
  ck_assert_int_eq(insertion_validator("2,", "+"), 0);
  ck_assert_int_eq(insertion_validator("(3*", ")"), 0);
  ck_assert_int_eq(insertion_validator("(5+5", ")"), 1);
  ck_assert_int_eq(insertion_validator("14", "sin("), 1);
  ck_assert_int_eq(insertion_validator("14,", "sin("), 0);
  ck_assert_int_eq(insertion_validator("3+", ","), 0);
  ck_assert_int_eq(insertion_validator("14,432", ","), 0);
  ck_assert_int_eq(insertion_validator("14432", ","), 1);
  ck_assert_int_eq(insertion_validator("2+", "2"), 1);
  ck_assert_int_eq(insertion_validator("14+0", "0"), 0);
  ck_assert_int_eq(insertion_validator("(3+3)", "7"), 0);
}
END_TEST

START_TEST(test_calc) {
  ck_assert_int_eq(calculate_validator("(3+x)"), 0);
  ck_assert_int_eq(calculate_validator("2+2"), 1);
  ck_assert_int_eq(calculate_validator("2+"), 0);
}
END_TEST

START_TEST(test_graph) {
  ck_assert_int_eq(graphic_validator("(3+x)"), 1);
  ck_assert_int_eq(graphic_validator("2+"), 0);
}
END_TEST

Suite* make_validators_suite(void) {
  Suite* s = suite_create("validators");
  TCase* tc = tcase_create("case");

  tcase_add_test(tc, test_insert);
  tcase_add_test(tc, test_calc);
  tcase_add_test(tc, test_graph);

  suite_add_tcase(s, tc);
  return s;
}
