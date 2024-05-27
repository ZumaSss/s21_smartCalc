#ifndef S21_SMART_CALC_H
#define S21_SMART_CALC_H

#include "limits.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

enum Status { OK, FAILURE, FAIL_X };

enum Priority {
  NULL_PRIORITY,
  FIRST_PRIORITY,
  SECOND_PRIORITY,
  THIRD_PRIORITY,
  FOURTH_PRIORITY,
  FIFTH_PRIORITY
};

enum values {
  NUMBER,
  OPEN_BRACKET,
  CLOSE_BRACKET,
  PLUS,
  MINUS,
  MUL,
  DIV,
  POW,
  MOD,
  X,
  UNAR_MINUS,
  COS,
  SIN,
  TAN,
  ACOS,
  ASIN,
  ATAN,
  SQRT,
  LN,
  LOG
};

struct StackNode {
  double value;
  int token;
  int priority;
  struct StackNode* next;
};

#ifdef __cplusplus
extern "C" {
#endif
struct StackNode* newNode(double value, int token, int priority);
int isEmpty(struct StackNode* root);
void push(struct StackNode** root, double value, int token, int priority);
void push_node(struct StackNode** root, struct StackNode* src);
int pop(struct StackNode** root);
double pop_num(struct StackNode** root);
void reverse_stack(struct StackNode** root, struct StackNode** reverse_root);
void peek(struct StackNode* root, double* value, int* token, int* priority);

void string_handling(char* str, char* res_str, int* flag_x);
int read_number_token(char* str, int* pos, char* num);
int get_token_at(char* str, int* pos);
int get_token_list(struct StackNode** root, char* str, double x);
double calculate_polish_notation(struct StackNode** root);
int calculate_operand(struct StackNode** num_stack,
                      struct StackNode** operand_stack, int token);
int driver(char* str, const char* x, char* output_str);
#ifdef __cplusplus
}
#endif
#endif