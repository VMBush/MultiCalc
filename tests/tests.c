#include "tests.h"

int main() {
  Suite *s_solve_polish = make_solve_polish_suite();
  Suite *s_helpers = make_helpers_suite();
  Suite *s_validators = make_validators_suite();

  SRunner *runner = srunner_create(s_solve_polish);
  srunner_add_suite(runner, s_helpers);
  srunner_add_suite(runner, s_validators);

  srunner_run_all(runner, CK_NORMAL);
  int failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
