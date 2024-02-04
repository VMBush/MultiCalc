#include "calculations.h"

// решает пришедшее (валидное!!) выражение в строковой нотации
char* solve_expression(char* src) {
  for (size_t i = 0; i < strlen(src); i++) {
    if (src[i] == ',') {
      src[i] = '.';
    }
  }
  lexemm_list* polish = to_polish(src);
  char* res = solve_polish(polish);

  for (size_t i = 0; i < strlen(res); i++) {
    if (res[i] == '.') {
      res[i] = ',';
    }
  }
  return res;
}

// преобразует строку в обратную польскую нотацию из лексемм
lexemm_list* to_polish(char* src) {
  // преобразуем строку в список лексемм без унарных операторов и добавив
  // умножение там, где это требуется
  lexemm_list* unsorted_lex = to_lexemm_list(src);
  unsorted_lex = add_missing_operators(unsorted_lex);

  // создаем списки для польки и стэка
  lexemm_list* polish = NULL;
  lexemm_list* buffer_stack = NULL;

  // перебираем элементы
  while (1) {
    // обработка числа
    if (unsorted_lex->lex.type == NUMBER) {
      polish = push_lexemm_stacked(unsorted_lex->lex, polish);
    }

    // обработка функции и открывающей скобки
    else if (unsorted_lex->lex.type == FUNC ||
             unsorted_lex->lex.type == OPEN_BR) {
      buffer_stack = push_lexemm_stacked(unsorted_lex->lex, buffer_stack);
    }

    // // обработка унарного оператора
    // else if (unsorted_lex->lex.type == OPERATOR && unsorted_lex->lex.operator
    // == 'v') {

    // }

    // обработка оператора
    else if (unsorted_lex->lex.type == OPERATOR) {
      while (buffer_stack && buffer_stack->lex.type == OPERATOR &&
             (buffer_stack->lex.operator_priority <
                  unsorted_lex->lex.operator_priority ||
              (buffer_stack->lex.operator_priority ==
                   unsorted_lex->lex.operator_priority &&
               strchr("+*", unsorted_lex->lex.operator)))) {
        lexemm popped_lex = pop_lexemm_stacked(&buffer_stack);
        polish = push_lexemm_stacked(popped_lex, polish);
      }
      buffer_stack = push_lexemm_stacked(unsorted_lex->lex, buffer_stack);
    }

    // обработка закрывающей скобки
    else if (unsorted_lex->lex.type == CLOSE_BR) {
      while (buffer_stack->lex.type != OPEN_BR) {
        lexemm popped_lex = pop_lexemm_stacked(&buffer_stack);
        polish = push_lexemm_stacked(popped_lex, polish);
      }
      pop_lexemm_stacked(&buffer_stack);
      if (buffer_stack && buffer_stack->lex.type == FUNC) {
        lexemm popped_lex = pop_lexemm_stacked(&buffer_stack);
        polish = push_lexemm_stacked(popped_lex, polish);
      }
    }

    // переходим к следующему элементу, если он есть
    if (!unsorted_lex->next) {
      break;
    } else {
      unsorted_lex = unsorted_lex->next;
    }
  }

  // дорабатываем стек
  while (buffer_stack) {
    lexemm popped_lex = pop_lexemm_stacked(&buffer_stack);
    polish = push_lexemm_stacked(popped_lex, polish);
  }

  while (unsorted_lex->prev) {
    unsorted_lex = unsorted_lex->prev;
  }
  while (polish->prev) {
    polish = polish->prev;
  }
  free_lexemm_list(unsorted_lex);
  return polish;
}

// преобразование строки в двусвязный список лексемм
lexemm_list* to_lexemm_list(char* src) {
  lexemm_list* prev = NULL;
  lexemm_list* current = NULL;
  while (*src != '\0') {
    // создаем новый элемент списка
    current = malloc(sizeof(lexemm_list));
    current->prev = prev;
    current->next = NULL;
    if (current->prev) {
      current->prev->next = current;
    }
    prev = current;

    // сколько символов строки считано
    int num = 0;
    scan_lexemm(src, &(current->lex), &num);
    src += num;
  }

  while (current->prev) {
    current = current->prev;
  }

  return current;
}

// считывание одной лексеммы
void scan_lexemm(char* src, lexemm* lex, int* sym_num) {
  *sym_num = 1;
  // число
  if (isnum(*src)) {
    sscanf(src, "%Lf%n", &(lex->number), sym_num);
    lex->type = NUMBER;
  }
  // скобки
  else if (*src == '(') {
    lex->type = OPEN_BR;
  } else if (*src == ')') {
    lex->type = CLOSE_BR;
  }
  // операторы
  else if (strchr("+-^v%/*", *src)) {
    lex->type = OPERATOR;
    lex->operator= * src;
    if (strchr("^v", *src)) {
      lex->operator_priority = 1;
    } else if (strchr("%/*", *src)) {
      lex->operator_priority = 2;
    } else if (strchr("+-", *src)) {
      lex->operator_priority = 3;
    }
  }
  // функции
  else {
    lex->type = FUNC;
    if (!strncmp(src, "ln(", 3)) {
      strcpy(lex->func, "ln");
      *sym_num = 2;
    } else if (!strncmp(src, "log(", 4) || !strncmp(src, "sin(", 4) ||
               !strncmp(src, "cos(", 4) || !strncmp(src, "tan(", 4)) {
      strncpy(lex->func, src, 3);
      lex->func[3] = '\0';
      *sym_num = 3;
    } else if (!strncmp(src, "asin(", 5) || !strncmp(src, "acos(", 5) ||
               !strncmp(src, "atan(", 5)) {
      strncpy(lex->func, src, 4);
      lex->func[4] = '\0';
      *sym_num = 4;
    }
  }
}

// меняет унарные операторы на бинарные и добавляет опущенное умножение
lexemm_list* add_missing_operators(lexemm_list* item) {
  // обработка на унарный оператор в начале строки
  if (!item->prev && item->lex.type == OPERATOR &&
      strchr("+-", item->lex.operator)) {
    lexemm_list* zero = malloc(sizeof(lexemm_list));
    zero->prev = NULL;
    zero->next = item;
    item->prev = zero;
    zero->lex.type = NUMBER;
    zero->lex.number = 0;
    item = zero;
  }

  while (item->next) {
    // проверяем унарные операторы и вставляем перед ними ноль
    if (item->lex.type == OPERATOR && strchr("+-", item->lex.operator) &&
        (item->prev->lex.type == FUNC || item->prev->lex.type == OPEN_BR)) {
      lexemm_list* zero = malloc(sizeof(lexemm_list));
      zero->prev = item->prev;
      item->prev->next = zero;

      zero->next = item;
      item->prev = zero;

      zero->lex.type = NUMBER;
      zero->lex.number = 0;
      item = zero;
    }

    // проверяем вставку умножения в случаях: [число или ")"] +
    // + ["(" или функция или число или корень]
    // стоит отметить, что вариант число/число невозможен из-за
    // валидации в других функциях
    else if ((item->lex.type == NUMBER || item->lex.type == CLOSE_BR) &&
             (item->next->lex.type == OPEN_BR || item->next->lex.type == FUNC ||
              item->next->lex.type == NUMBER ||
              (item->next->lex.type == OPERATOR &&
               item->next->lex.operator== 'v'))) {
      lexemm_list* mul = malloc(sizeof(lexemm_list));
      mul->prev = item;
      mul->next = item->next;

      mul->prev->next = mul;
      mul->next->prev = mul;

      mul->lex.type = OPERATOR;
      mul->lex.operator= '*';
      mul->lex.operator_priority = 2;
      item = mul;
    }
    item = item->next;
  }

  while (item->prev) {
    item = item->prev;
  }

  return item;
}

// решает корректное польское выражение
char* solve_polish(lexemm_list* polish) {
  char* result = malloc(1000 * sizeof(char));

  // вычисляем
  while (polish->next) {
    if (polish->next->lex.type == NUMBER) {
      polish = polish->next;
    } else if (polish->next->lex.type == OPERATOR) {
      lexemm lex_oper = pop_n_free_lexemm(polish->next);
      // если корень то просто считаем
      if (lex_oper.operator== 'v') {
        polish->lex.number = sqrt(polish->lex.number);
      }
      // если бинарный оператор, пропадают и левая, и правая лексемма
      else {
        lexemm lex_left_num = pop_n_free_lexemm(polish->prev);
        if (lex_oper.operator== '^') {
          polish->lex.number = powl(lex_left_num.number, polish->lex.number);
        } else if (lex_oper.operator== '%') {
          polish->lex.number = fmodl(lex_left_num.number, polish->lex.number);
        } else if (lex_oper.operator== '*') {
          polish->lex.number = lex_left_num.number * polish->lex.number;
        } else if (lex_oper.operator== '/') {
          polish->lex.number = lex_left_num.number / polish->lex.number;
        } else if (lex_oper.operator== '+') {
          polish->lex.number = lex_left_num.number + polish->lex.number;
        } else if (lex_oper.operator== '-') {
          polish->lex.number = lex_left_num.number - polish->lex.number;
        }
      }
    }

    else if (polish->next->lex.type == FUNC) {
      lexemm lex_func = pop_n_free_lexemm(polish->next);

      if (!strcmp(lex_func.func, "ln")) {
        polish->lex.number = logl(polish->lex.number);
      } else if (!strcmp(lex_func.func, "log")) {
        polish->lex.number = log10l(polish->lex.number);
      } else if (!strcmp(lex_func.func, "sin")) {
        polish->lex.number = sinl(polish->lex.number);
      } else if (!strcmp(lex_func.func, "cos")) {
        polish->lex.number = cosl(polish->lex.number);
      } else if (!strcmp(lex_func.func, "tan")) {
        polish->lex.number = tanl(polish->lex.number);
      } else if (!strcmp(lex_func.func, "asin")) {
        polish->lex.number = asinl(polish->lex.number);
      } else if (!strcmp(lex_func.func, "acos")) {
        polish->lex.number = acos(polish->lex.number);
      } else if (!strcmp(lex_func.func, "atan")) {
        polish->lex.number = atanl(polish->lex.number);
      }
    }
  }
  if (polish->lex.number != polish->lex.number) {
    sprintf(result, "ERROR");
  } else if (polish->lex.number == INFINITY) {
    sprintf(result, "inf");
  } else if (polish->lex.number == -INFINITY) {
    sprintf(result, "-inf");
  } else {
    snprintf(result, 999, "%.25Lf", polish->lex.number);
  }
  // убираем незначащие нули
  if (strchr(result, '.')) {
    while (result[strlen(result) - 1] == '0') {
      result[strlen(result) - 1] = '\0';
    }
    if (result[strlen(result) - 1] == '.') {
      result[strlen(result) - 1] = '\0';
    }
  }

  free(polish);
  return result;
}

void free_lexemm_list(lexemm_list* list) {
  if (list) {
    if (list->next) {
      free_lexemm_list(list->next);
    }
    free(list);
  }
}

// добавляет на вершину стека лексемму
lexemm_list* push_lexemm_stacked(lexemm lex, lexemm_list* list) {
  lexemm_list* new = malloc(sizeof(lexemm_list));
  if (list) {
    list->next = new;
  }
  new->next = NULL;
  new->lex = lex;
  new->prev = list;

  return new;
}

// возвращает лексемму с вершины стека и чистит элемент
lexemm pop_lexemm_stacked(lexemm_list** list) {
  lexemm_list* top = *list;
  lexemm lex = top->lex;

  *list = top->prev;
  free(top);
  if (*list) {
    (*list)->next = NULL;
  }

  return lex;
}

lexemm pop_n_free_lexemm(lexemm_list* el) {
  lexemm lex = el->lex;

  if (el->next) {
    el->next->prev = el->prev;
  }
  if (el->prev) {
    el->prev->next = el->next;
  }

  free(el);
  return lex;
}
