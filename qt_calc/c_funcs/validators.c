#include "../smartcalc.h"

int insertion_validator(char* dest, char* el) {
  int res = 1;
  int dest_size = strlen(dest);
  char* last = dest + dest_size - 1;
  // пустая строка
  if (dest_size == 1 && dest[0] == '0') {
    res = strchr("0)", el[0]) ? 0 : 1;
  }

  // x и (
  else if (el[0] == 'x') {
    return strchr("x,", *last) ? 0 : 1;
  } else if (el[0] == '(') {
    return strchr(",", *last) ? 0 : 1;
  }

  // обработка чисел и запятой
  else if (isnum(el[0]) || el[0] == ',') {
    res = insertion_num_validator(dest, el);
  }

  // бинарные операторы
  else if (strchr("^%/*", el[0])) {
    if (strchr("(,", *last) || (strchr("+-", *last) && *(last - 1) == '(')) {
      res = 0;
    } else {
      res = 1;
    }
  }

  // унарные операторы
  else if (strchr("+-", el[0])) {
    if (*last == ',') {
      res = 0;
    } else {
      res = 1;
    }
  }

  // )
  else if (el[0] == ')') {
    if (strchr("+-^v%/*(", *last) || openned_brackets_count(dest) < 1) {
      res = 0;
    } else {
      res = 1;
    }
  }

  // букавы и унарный корень
  else {
    if (*last == ',') {
      res = 0;
    } else {
      res = 1;
    }
  }
  return res;
}

// обработка чисел и запятой
int insertion_num_validator(char* dest, char* el) {
  int res = 1;
  char* last = dest + strlen(dest) - 1;

  // обработка запятой
  if (el[0] == ',') {
    // если не идет ввод числа
    if (!isnum(*last)) {
      res = 0;
    } else {
      // проверка если левее уже есть запятая
      while (isnum(*last) && last - dest) {
        last--;
      }
      if (*last == ',') {
        // запятая есть
        res = 0;
      } else {
        // запятой нет
        res = 1;
      }
    }
  } else {  // обработка чисел
    // если это просто ноль
    if (*last == '0' && !(isnum(*(last - 1)) || *(last - 1) == ',')) {
      res = 0;
    } else if (strchr("0123456789,(^v%/*+-", *last)) {
      // если вводится число или стоит оператор
      res = 1;
    } else {
      res = 0;
    }
  }

  return res;
}

int calculate_validator(char* dest) {
  int res = 1;
  int dest_size = strlen(dest);
  char* last = dest + dest_size - 1;
  if (strchr(dest, 'x')) {
    res = 0;
  } else if (strchr(")", *last) || isnum(*last)) {
    res = 1;
  } else {
    res = 0;
  }

  return res;
}

int graphic_validator(char* dest) {
  int res = 1;
  int dest_size = strlen(dest);
  char* last = dest + dest_size - 1;
  if (strchr("x)", *last) || isnum(*last)) {
    res = 1;
  } else {
    res = 0;
  }

  return res;
}