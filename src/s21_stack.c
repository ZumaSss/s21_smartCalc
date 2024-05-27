#include "s21_smart_calc.h"

struct StackNode* newNode(double value, int token, int priority) {
  struct StackNode* stackNode =
      (struct StackNode*)malloc(sizeof(struct StackNode));
  stackNode->value = value;
  stackNode->token = token;
  stackNode->priority = priority;
  stackNode->next = NULL;
  return stackNode;
}

int isEmpty(struct StackNode* root) { return !root; }

void push(struct StackNode** root, double value, int token, int priority) {
  struct StackNode* stackNode = newNode(value, token, priority);
  stackNode->next = *root;
  *root = stackNode;
}

void push_node(struct StackNode** root, struct StackNode* src) {
  if (isEmpty(src)) return;
  push(root, src->value, src->token, src->priority);
}

int pop(struct StackNode** root) {
  if (isEmpty(*root)) return INT_MIN;
  struct StackNode* temp = *root;
  *root = (*root)->next;
  int popped = temp->token;
  free(temp);

  return popped;
}

double pop_num(struct StackNode** root) {
  if (isEmpty(*root)) return INT_MIN;
  struct StackNode* temp = *root;
  *root = (*root)->next;
  double popped = temp->value;
  free(temp);

  return popped;
}

void reverse_stack(struct StackNode** root, struct StackNode** reverse_root) {
  while (!isEmpty(*root)) {
    push(reverse_root, (*root)->value, (*root)->token, (*root)->priority);
    pop(root);
  }
}

void peek(struct StackNode* root, double* value, int* token, int* priority) {
  if (isEmpty(root) || !value || !token || !priority) return;
  *value = root->value;
  *token = root->token;
  *priority = root->priority;
}