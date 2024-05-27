#include "s21_smart_calc.h"

// int main() {
//   char str[256] = "sin()";
//   char output_str[256];

//   if (driver(str, "1", output_str)) return 1;

//   printf("%s\n", output_str);

//   return 0;
// }

int driver(char* str, const char* input_x, char* output_str) {
  char res_str[256];
  struct StackNode* root = NULL;
  struct StackNode* reverse_root = NULL;
  int flag_x = 0;
  int res = OK;

  string_handling(str, res_str, &flag_x);
  if (flag_x && input_x == NULL)
    res = FAIL_X;
  else if (flag_x && input_x != NULL)
    res = get_token_list(&root, res_str, atof(input_x));
  else
    res = get_token_list(&root, res_str, 0);
  if (!res) {
    reverse_stack(&root, &reverse_root);
    snprintf(output_str, 255, "%.7f", calculate_polish_notation(&reverse_root));
  }

  while (!isEmpty(root)) pop(&root);
  free(root);
  while (!isEmpty(reverse_root)) pop(&reverse_root);
  free(reverse_root);

  return res;
}

int calculate_operand(struct StackNode** num_stack,
                      struct StackNode** operand_stack, int token) {
  double num1, num2;
  int res = OK;
  if (token > X) {
    num1 = pop_num(num_stack);
    if (num1 == INT_MIN)
      res = FAILURE;
    else {
      switch (token) {
        case UNAR_MINUS:
          num1 *= (-1);
          break;
        case COS:
          num1 = cos(num1);
          break;
        case SIN:
          num1 = sin(num1);
          break;
        case TAN:
          num1 = tan(num1);
          break;
        case ACOS:
          num1 = acos(num1);
          break;
        case ASIN:
          num1 = asin(num1);
          break;
        case ATAN:
          num1 = atan(num1);
          break;
        case SQRT:
          num1 = sqrt(num1);
          break;
        case LN:
          num1 = log(num1);
          break;
        case LOG:
          num1 = log10(num1);
          break;
      }
      push(num_stack, num1, NUMBER, NULL_PRIORITY);
    }
  } else {
    num1 = pop_num(num_stack);
    num2 = pop_num(num_stack);
    if (num1 == INT_MIN || num2 == INT_MIN)
      res = FAILURE;
    else {
      switch (token) {
        case PLUS:
          num2 += num1;
          break;
        case MINUS:
          num2 -= num1;
          break;
        case MUL:
          num2 *= num1;
          break;
        case DIV:
          num2 /= num1;
          break;
        case POW:
          num2 = pow(num2, num1);
          break;
        case MOD:
          num2 = fmod(num2, num1);
          break;
      }
      push(num_stack, num2, NUMBER, NULL_PRIORITY);
    }
  }
  pop(operand_stack);
  return res;
}

double calculate_polish_notation(struct StackNode** root) {
  double value_root = 0, value_temp = 0;
  double res = 0;
  int token_root = 0, token_temp = 0, priority_root = 0, priority_temp = 0;
  struct StackNode* num_stack = NULL;
  struct StackNode* operand_stack = NULL;

  while (!isEmpty(*root) && res != NAN) {
    peek(*root, &value_root, &token_root, &priority_root);
    if (token_root == NUMBER || token_root == X)
      push_node(&num_stack, *root);
    else if (token_root == OPEN_BRACKET)
      push_node(&operand_stack, *root);
    else if (token_root == CLOSE_BRACKET) {
      peek(operand_stack, &value_temp, &token_temp, &priority_temp);
      while (token_temp != OPEN_BRACKET && !isEmpty(operand_stack)) {
        if (!calculate_operand(&num_stack, &operand_stack, token_temp))
          peek(operand_stack, &value_temp, &token_temp, &priority_temp);
        else {
          res = NAN;
          break;
        }
      }
      pop(&operand_stack);
    } else {
      peek(operand_stack, &value_temp, &token_temp, &priority_temp);
      while (priority_temp >= priority_root && token_temp != OPEN_BRACKET &&
             !isEmpty(operand_stack)) {
        if (!calculate_operand(&num_stack, &operand_stack, token_temp))
          peek(operand_stack, &value_temp, &token_temp, &priority_temp);
        else {
          res = NAN;
          break;
        }
      }
      push_node(&operand_stack, *root);
    }
    pop(root);
  }
  while (!isEmpty(operand_stack)) {
    peek(operand_stack, &value_temp, &token_temp, &priority_temp);
    calculate_operand(&num_stack, &operand_stack, token_temp);
  }
  if (!isEmpty(num_stack))
    res = num_stack->value;
  else
    res = NAN;
  pop(&num_stack);

  while (!isEmpty(operand_stack)) pop(&operand_stack);
  free(operand_stack);
  while (!isEmpty(num_stack)) pop(&num_stack);
  free(num_stack);

  return res;
}

int read_number_token(char* str, int* pos, char* num) {
  int res = OK, flag = 0, index = 0;
  while ((str[*pos] >= '0' && str[*pos] <= '9') || str[*pos] == '.') {
    if (str[*pos] == '.') {
      if (flag) {
        res = FAILURE;
        break;
      } else
        flag = 1;
    }
    num[index++] = str[*pos];
    *pos += 1;
  }
  num[index] = '\0';

  if (!res &&
      (str[*pos] != '*' && str[*pos] != '/' && str[*pos] != '-' &&
       str[*pos] != '+' && str[*pos] != '^' && str[*pos] != '(' &&
       str[*pos] != ')' && str[*pos] != '\0') &&
      (str[*pos] != 'm' && str[*pos + 1] != 'o' && str[*pos + 2] != 'd'))
    res = FAILURE;

  return res;
}

int get_token_list(struct StackNode** root, char* str, double x) {
  int res = OK, pos = 0, bracket_balance = 0;
  int token = 0, priority = 0;
  double value = 0;
  char* num = NULL;

  if (str[pos] == '\0') res = FAILURE;

  while (str[pos] != '\0' && res == OK) {
    switch (get_token_at(str, &pos)) {
      case NUMBER:
        num = (char*)malloc(256 * sizeof(char));
        res = read_number_token(str, &pos, num);
        if (!res) {
          value = atof(num);
          push(root, value, NUMBER, NULL_PRIORITY);
        }
        free(num);
        num = NULL;
        break;
      case OPEN_BRACKET:
        push(root, 0, OPEN_BRACKET, FIFTH_PRIORITY);
        bracket_balance++;
        break;
      case CLOSE_BRACKET:
        push(root, 0, CLOSE_BRACKET, FIFTH_PRIORITY);
        bracket_balance--;
        break;
      case PLUS:
        peek(*root, &value, &token, &priority);
        if (token != NUMBER && token != CLOSE_BRACKET)
          res = FAILURE;
        else
          push(root, 0, PLUS, FIRST_PRIORITY);
        break;
      case MINUS:
        push(root, 0, MINUS, FIRST_PRIORITY);
        break;
      case MUL:
        peek(*root, &value, &token, &priority);
        if ((token != NUMBER && token != CLOSE_BRACKET) || isEmpty(*root))
          res = FAILURE;
        else
          push(root, 0, MUL, SECOND_PRIORITY);
        break;
      case DIV:
        peek(*root, &value, &token, &priority);
        if ((token != NUMBER && token != CLOSE_BRACKET) || isEmpty(*root))
          res = FAILURE;
        else
          push(root, 0, DIV, SECOND_PRIORITY);
        break;
      case POW:
        peek(*root, &value, &token, &priority);
        if ((token != NUMBER && token != CLOSE_BRACKET) || isEmpty(*root))
          res = FAILURE;
        else
          push(root, 0, POW, THIRD_PRIORITY);
        break;
      case MOD:
        peek(*root, &value, &token, &priority);
        if ((token != NUMBER && token != CLOSE_BRACKET) || isEmpty(*root))
          res = FAILURE;
        else
          push(root, 0, MOD, SECOND_PRIORITY);
        break;
      case X:
        push(root, x, NUMBER, NULL_PRIORITY);
        break;
      case UNAR_MINUS:
        push(root, 0, UNAR_MINUS, FOURTH_PRIORITY);
        break;
      case COS:
        push(root, 0, COS, FOURTH_PRIORITY);
        break;
      case SIN:
        push(root, 0, SIN, FOURTH_PRIORITY);
        break;
      case TAN:
        push(root, 0, TAN, FOURTH_PRIORITY);
        break;
      case ACOS:
        push(root, 0, ACOS, FOURTH_PRIORITY);
        break;
      case ASIN:
        push(root, 0, ASIN, FOURTH_PRIORITY);
        break;
      case ATAN:
        push(root, 0, ATAN, FOURTH_PRIORITY);
        break;
      case SQRT:
        push(root, 0, SQRT, THIRD_PRIORITY);
        break;
      case LN:
        push(root, 0, LN, FOURTH_PRIORITY);
        break;
      case LOG:
        push(root, 0, LOG, FOURTH_PRIORITY);
        break;
      default:
        res = FAILURE;
        break;
    }
  }

  if (bracket_balance != 0 && !res) res = FAILURE;

  return res;
}

void string_handling(char* str, char* res_str, int* flag_x) {
  char str_temp[512] = {0};
  int index_temp = 0;
  int index = 0;
  for (size_t i = 0; i < strlen(str); i++) {
    if (str[i] == ' ') continue;
    str_temp[index_temp++] = str[i];
  }

  for (size_t i = 0; i < strlen(str_temp); i++) {
    if ((str_temp[i] == ')' && str_temp[i + 1] == 'x') ||
        ((str_temp[i] >= '0' && str_temp[i] <= '9') &&
         str_temp[i + 1] == 'x') ||
        (str_temp[i] == 'x' && str_temp[i + 1] == 'x') ||
        ((str_temp[i + 1] >= '0' && str_temp[i + 1] <= '9') &&
         str_temp[i] == ')') ||
        (str_temp[i] == ')' && str_temp[i + 1] == '(') ||
        (((str_temp[i] >= '0' && str_temp[i] <= '9') || str_temp[i] == ')') &&
         (str_temp[i + 1] == 'c' || str_temp[i + 1] == 's' ||
          str_temp[i + 1] == 't' || str_temp[i + 1] == 'a' ||
          str_temp[i + 1] == 'l'))) {
      res_str[index++] = str_temp[i];
      res_str[index++] = '*';
    } else if ((str_temp[i] == '/' || str_temp[i] == '*' ||
                str_temp[i] == '-' || str_temp[i] == '^') &&
               str_temp[i + 1] == '+') {
      res_str[index++] = str_temp[i];
      i++;
    } else if ((str_temp[0] == '+' && i == 0) ||
               (str_temp[0] == '-' && i == 0)) {
      res_str[index++] = '0';
      res_str[index++] = str_temp[i];
    } else if (str_temp[i] == '(' && str_temp[i + 1] == '+') {
      res_str[index++] = str_temp[i];
      i++;
    } else if (str_temp[i] == '(' && str_temp[i + 1] == '-') {
      res_str[index++] = str_temp[i];
      res_str[index++] = '0';
    } else if (str_temp[i] == 'x') {
      *flag_x = 1;
      res_str[index++] = str_temp[i];
    } else
      res_str[index++] = str_temp[i];
  }
  res_str[index++] = '\0';
}

int get_token_at(char* str, int* pos) {
  int token;

  if (str[*pos] > 47 && str[*pos] < 58) {
    token = 0;
  } else if (str[*pos] == '(') {
    token = 1;
    *pos += 1;
  } else if (str[*pos] == ')') {
    token = 2;
    *pos += 1;
  } else if (str[*pos] == '+') {
    token = 3;
    *pos += 1;
  } else if (str[*pos] == '-') {
    if (!(str[*pos - 1] > 47 && str[*pos - 1] < 58) && str[*pos - 1] != ')' &&
        str[*pos - 1] != 'x')
      token = 10;
    else
      token = 4;
    *pos += 1;
  } else if (str[*pos] == '*') {
    token = 5;
    *pos += 1;
  } else if (str[*pos] == '/') {
    token = 6;
    *pos += 1;
  } else if (str[*pos] == '^') {
    token = 7;
    *pos += 1;
  } else if (str[*pos] == 'm' && str[*pos + 1] == 'o' && str[*pos + 2] == 'd') {
    token = 8;
    *pos += 3;
  } else if (str[*pos] == 'x') {
    token = 9;
    *pos += 1;
  } else if (str[*pos] == 'c' && str[*pos + 1] == 'o' && str[*pos + 2] == 's') {
    token = 11;
    *pos += 3;
  } else if (str[*pos] == 's' && str[*pos + 1] == 'i' && str[*pos + 2] == 'n') {
    token = 12;
    *pos += 3;
  } else if (str[*pos] == 't' && str[*pos + 1] == 'a' && str[*pos + 2] == 'n') {
    token = 13;
    *pos += 3;
  } else if (str[*pos] == 'a' && str[*pos + 1] == 'c' && str[*pos + 2] == 'o' &&
             str[*pos + 3] == 's') {
    token = 14;
    *pos += 4;
  } else if (str[*pos] == 'a' && str[*pos + 1] == 's' && str[*pos + 2] == 'i' &&
             str[*pos + 3] == 'n') {
    token = 15;
    *pos += 4;
  } else if (str[*pos] == 'a' && str[*pos + 1] == 't' && str[*pos + 2] == 'a' &&
             str[*pos + 3] == 'n') {
    token = 16;
    *pos += 4;
  } else if (str[*pos] == 's' && str[*pos + 1] == 'q' && str[*pos + 2] == 'r' &&
             str[*pos + 3] == 't') {
    token = 17;
    *pos += 4;
  } else if (str[*pos] == 'l' && str[*pos + 1] == 'n') {
    token = 18;
    *pos += 2;
  } else if (str[*pos] == 'l' && str[*pos + 1] == 'o' && str[*pos + 2] == 'g') {
    token = 19;
    *pos += 3;
  } else
    token = -1;

  return token;
}