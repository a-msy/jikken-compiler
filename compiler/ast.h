#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
typedef enum{
  IDENT_AST=0,
  IDENTS_AST,
  NUMBER_AST,
  COND_AST,
  GLT_AST,
  LTE_AST,
  LT_AST,
  GT_AST,
  EQ_AST,
  ADD_AST,
  DIV_AST,
  MUL_AST,
  SUB_AST,
  SUR_AST,
  DECRE_AST,
  INCRE_AST,
  OP_INCRE,
  OP_DECRE,
  ASSIGNMENT_STMT_AST,
  ELSE_AST,
  IF_STMT_AST,
  ELIF_STMT_AST,
  ELSE_STMT_AST,
  FOR_LOOP_AST,
  WHILE_LOOP_AST,
  ARRAY_AST,
  DEFINE_AST,
  FUNCCALL_AST,
  FUNC_AST,
  PROGRAM_AST,
  DECLARATIONS_AST,
  DECL_STATEMENT_AST,
  STATEMENTS_AST,
  STATEMENT_AST,
  ASSIGNMENT_AST,
  EXPRESSION_AST,
  TERM_AST,
  FACTOR_AST,
  ARGUMENT_DCLLIST_AST,
  ARGUMENT_CALLLIST_AST,
  FUNCTION_DCL_AST,
  FUNCTION_CALL_AST,
  ARRAY_INDEX_AST,
  BREAK_AST,
  CREMENT_AST,
  VAR_AST,
  LOOP_STMT_AST,
  COND_STMT_AST,
  DEFINE_ARRAY_AST,
  ARGUMENT_DCL_AST,
  OP_EQ,
  OP_LT,
  OP_GT,
  OP_LTE,
  OP_GLT,
  OP_MUL,
  OP_ADD,
  OP_SUB,
  OP_DIV,
  IF_AST,
  OP_SUR,
} NType;

typedef struct node{
  NType type; // ノードの種別 コード生成時等で必須
  int value;  // 整数のノード型の場合の整数値 
  char* variable; // 変数のノード型の場合の文字列
  struct node *child; 
  struct node *brother;
} Node;

typedef struct variable {
  char* name;
  int offset;
  int size;
} Var;

Node* build_node1(NType t, Node* p1);
Node* build_node2(NType t, Node* p1, Node* p2);
Node* build_node3(NType t, Node* p1, Node* p2, Node* p3);
Node* build_node4(NType t, Node* p1, Node* p2, Node* p3, Node* p4);
Node* build_ident_node(NType t, char* s);
Node* build_num_node(NType t, int n);
Node *build_array_node(NType t, char *s, Node *index);

void printTree(Node *n);
void printNode(Node *n);
void printVars(int length);
void printVar(Var *var);

void initialize(FILE *fp);
void settle(FILE *fp);
void hasFileName(int arg);

void genCode(Node *n, FILE *fp);
void genCodeNumberOrIdent(Node *n, char *reg, FILE *fp);
int getOffset(Node *n);
void assignment(Node *n, FILE *fp);
void expression(Node *n, FILE *fp);
int isOperator(Node *n);
int isIdentNumber(Node *n);
int isOperator(Node *n);
void OP(Node *n, FILE *fp, int left, int right, int result);
void condition(Node *n, FILE *fp);
void branch(Node *n, FILE *fp, char *jump);
void ifstmt(Node *n, FILE *fp);
void elsestmt(Node *n, FILE *fp);
void expressionSub(Node *n, FILE *fp);
int expressionSubHeap(Node *n, FILE *fp, char *t, int heap_addr);