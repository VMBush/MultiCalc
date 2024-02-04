#include "../smartcalc.h"

int isnum(char el) { return strchr("0123456789", el) ? 1 : 0; }

int openned_brackets_count(char* str) {
  int cntr = 0;
  for (size_t i = 0; i < strlen(str); i++) {
    if (str[i] == '(') {
      cntr++;
    } else if (str[i] == ')') {
      cntr--;
    }
  }
  return cntr;
}

void delete_last_symbol(char* str) {
  int str_size = strlen(str);
  char* last = str + str_size - 1;
  if (str_size == 1) {
    str[0] = '0';
  } else if (strchr("0123456789,x)^v%/*+-", *last)) {
    *last = '\0';
  } else if (*last == '(') {
    if (!strchr("ngs", *(last - 1))) {
      *last = '\0';
    } else {
      if (!strcmp(last - 2, "ln(")) {
        last -= 2;
      } else if (str_size > 4 &&
                 (!strcmp(last - 4, "asin(") || !strcmp(last - 4, "acos(") ||
                  !strcmp(last - 4, "atan("))) {
        last -= 4;
      } else if (!strcmp(last - 3, "log(") || !strcmp(last - 3, "sin(") ||
                 !strcmp(last - 3, "cos(") || !strcmp(last - 3, "tan(")) {
        last -= 3;
      }

      *last = '\0';
    }
  }
  if (strlen(str) == 0) {
    strcpy(str, "0");
  }
}