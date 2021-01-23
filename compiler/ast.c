#include "ast.h"
/*
命名規則：キャメルケース
has, get, is, genを接頭辞に使う
*/
Node *top; // 抽象構文木のルートノードを受け取るための変数
Var *variables[64];
char *typeChar[64];
int vars = 0;
int offset = 0;
int loops = 0;
int branches = 0;
int ifs = 0;

typedef enum
{
  FALSE = 0,
  TRUE = 1,
} BOOL;

Node *build_node1(NType t, Node *p1)
{
  Node *p;
  p = (Node *)malloc(sizeof(Node));
  if (p == NULL)
  {
    yyerror("out of memory");
  }
  p->type = t;
  p->child = p1;
  return p;
}

Node *build_node2(NType t, Node *p1, Node *p2)
{
  Node *p;
  p = (Node *)malloc(sizeof(Node));
  if (p == NULL)
  {
    yyerror("out of memory");
  }
  p->type = t;
  p->child = p1;
  p->child->brother = p2;
  return p;
}

Node *build_node3(NType t, Node *p1, Node *p2, Node *p3)
{
  Node *p;
  p = (Node *)malloc(sizeof(Node));
  if (p == NULL)
  {
    yyerror("out of memory");
  }
  p->type = t;
  p->child = p1;
  p->child->brother = p2;
  p->child->brother->brother = p3;
  return p;
}

Node *build_node4(NType t, Node *p1, Node *p2, Node *p3, Node *p4)
{
  Node *p;
  p = (Node *)malloc(sizeof(Node));
  if (p == NULL)
  {
    yyerror("out of memory");
  }
  p->type = t;
  p->child = p1;
  p->child->brother = p2;
  p->child->brother->brother = p3;
  p->child->brother->brother->brother = p4;
  return p;
}

Node *build_ident_node(NType t, char *s)
{
  Node *p;
  p = (Node *)malloc(sizeof(Node));
  if (p == NULL)
  {
    yyerror("out of memory");
  }
  p->type = t;
  p->variable = s;
  p->child = NULL;
  return p;
}

Node *build_num_node(NType t, int n)
{
  Node *p;
  p = (Node *)malloc(sizeof(Node));
  if (p == NULL)
  {
    yyerror("out of memory");
  }
  p->type = t;
  p->value = n;
  p->child = NULL;
  return p;
}

Node *build_array_node(NType t, char *s, Node *index)
{
  Node *p;
  p = (Node *)malloc(sizeof(Node));
  if (p == NULL)
  {
    yyerror("out of memory");
  }
  p->type = t;
  p->variable = s;
  p->child = index;
  return p;
}

int main(int argc, char *argv[])
{
  if (yyparse())
  {
    fprintf(stderr, "Error !\n");
    return 1;
  }
  else
  {
    fprintf(stderr, "No Syntax Error.\n");
    printf("===== printTree START =====\n");
    printTree(top);
    printf("===== printTree END =====\n");
    hasFileName(argc);
    FILE *fp;
    fp = fopen(strcat(argv[1], ".s"), "w");
    if (fp == NULL)
    {                                   // オープンに失敗した場合
      fprintf(stderr, "cannot open\n"); // エラーメッセージを出して
      exit(1);                          // 異常終了
    }
    initialize(fp);
    genCode(top, fp);
    settle(fp);
    fprintf(stderr, "DONE\n");
    fclose(fp);
    return 0;
  }
}

void hasFileName(int arg)
{
  if (arg < 2)
  {
    printf("No FileName\n");
    exit(1);
  }
}
void initialize(FILE *fp)
{
  fprintf(fp, "    INITIAL_GP = 0x10008000\n");
  fprintf(fp, "    INITIAL_SP = 0x7ffffffc\n");
  fprintf(fp, "    stop_service = 99\n");
  fprintf(fp, "\n");
  fprintf(fp, "    .text\n");
  fprintf(fp, "init:");
  fprintf(fp, "\n");
  fprintf(fp, "    la $gp, INITIAL_GP\n");
  fprintf(fp, "    la $sp, INITIAL_SP\n");
  fprintf(fp, "    jal main\n");
  fprintf(fp, "    nop\n");
  fprintf(fp, "    li $v0, stop_service\n");
  fprintf(fp, "    syscall\n");
  fprintf(fp, "    nop\n");
  fprintf(fp, "stop:\n");
  fprintf(fp, "    j stop\n");
  fprintf(fp, "    nop\n");
  fprintf(fp, "\n");
  fprintf(fp, "    .text 0x00001000\n");
  fprintf(fp, "main:\n ");
  fprintf(fp, "   la $t0, RESULT\n");
}

void settle(FILE *fp)
{
  fprintf(fp, "\n    jr $ra\n");
  fprintf(fp, "    nop\n");
  fprintf(fp, "    .data 0x10004000\n");
  fprintf(fp, "RESULT:\n");
}

void printNode(Node *n)
{
  if (n != NULL)
  {
    printf("type : %s\n", typeChar[n->type]);
    if (n->type == NUMBER_AST)
    {
      printf("value : %d\n", n->value);
    }
    if (n->type == IDENT_AST)
    {
      printf("variable : %s\n", n->variable);
    }
    printf("\n");
  }
}

void printTree(Node *n)
{
  if (n != NULL)
  {
    printNode(n);
    if (n->child != NULL)
    {
      printTree(n->child);
    }
    if (n->brother != NULL)
    {
      printTree(n->brother);
    }
  }
}

void genCode(Node *n, FILE *fp)
{
  printf("genCode\n");
  printNode(n);
  if (n->type == IDENT_AST)
  {
    Var *v;
    v = (Var *)malloc(sizeof(Var));
    v->offset = offset;
    v->name = n->variable;
    v->size = 1;
    variables[vars] = v;
    vars++;
    offset = offset + (v->size * 4);
    printVars(vars);
  }
  else if (n->type == ASSIGNMENT_STMT_AST)
  {
    fprintf(fp,"#s assignment %s\n",n->child->variable);
    expression(n->child->brother, fp);
    assignment(n, fp);
    fprintf(fp,"#e assignment\n");
  }
  else if (n->type == WHILE_LOOP_AST)
  {
    int numLoop = loops;
    loops++;
    fprintf(fp, "$WHILE%d:\n", numLoop);
    condition(n->child, fp);
    char jump[10];
    sprintf(jump, "$ENDWHILE%d", numLoop);
    fprintf(fp,"# s while branch\n");
    branch(n->child, fp, jump);
    fprintf(fp,"# e while branch\n");
    genCode(n->child->brother, fp);
    fprintf(fp, "    j $WHILE%d\n", numLoop);
    fprintf(fp, "    nop\n");
    fprintf(fp, "$ENDWHILE%d:\n", numLoop);
  }
  else if (n->type == IF_STMT_AST)
  {
    fprintf(fp,"#s if stmt\n");
    ifstmt(n, fp);
    fprintf(fp, "$END%d:\n", ifs);
    fprintf(fp,"#e if stmt\n");
    ifs++;
  }
  else
  {
    if (n->child != NULL)
    {
      genCode(n->child, fp);
    }

    if (n->brother != NULL)
    {
      genCode(n->brother, fp);
    }
  }
}

int isOperator(Node *n)
{
  if (n == NULL)
  {
    return FALSE;
  }
  else if(n->type == ADD_AST || n->type == SUB_AST || n->type == MUL_AST || n->type == DIV_AST)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
int isIdentNumber(Node *n)
{
  printf("isIdentNumber :");
  printNode(n);
  if (n == NULL)
  {
    return FALSE;
  }
  else if(n->type == IDENT_AST || n->type == NUMBER_AST)
  {
    printf("TRUE\n");
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
void expression(Node *n, FILE *fp)
{
  printf("expression n :");
  printNode(n);
  printf("expression n->child :");
  printNode(n->child);
  if (isOperator(n) == TRUE)
  {
    if(isIdentNumber(n->child) == TRUE)
    {
      genCodeNumberOrIdent(n->child, "t1", fp);
    }
    else
    {
      expression(n->child, fp);
      fprintf(fp, "    add $t1, $v0, $zero\n");
    }

    if(isIdentNumber(n->child->brother) == TRUE)
    {
      genCodeNumberOrIdent(n->child->brother, "t3", fp);
    }
    else
    {
      expression(n->child->brother, fp);
      fprintf(fp, "    add $t3, $v0, $zero\n");
    }
    OP(n, fp);
  }
  else if (isIdentNumber(n) == TRUE)
  {
    genCodeNumberOrIdent(n, "v0", fp);
  }
  else
  {
    expression(n->child, fp);
  }
  return ;
}

void genCodeNumberOrIdent(Node *n, char *reg, FILE *fp)
{
  if (n->type == IDENT_AST)
  {
    printf("genCode Ident\n");
    int offset = getOffset(n);
    fprintf(fp, "    lw $%s, %d($t0)\n", reg, offset);
    fprintf(fp, "    nop\n");
  }
  else if (n->type == NUMBER_AST)
  {
    printf("genCode Number\n");
    fprintf(fp, "    li $%s, %d\n", reg, n->value);
  }
  return;
}

int getOffset(Node *n)
{
  int i = 0;
  printNode(n);
  for (i = 0; i < vars; i++)
  {
    if (strcmp(variables[i]->name, n->variable) == 0) //宣言にあるかどうか
    {
      return variables[i]->offset;
    }
  }
  printf("Error!!! Undefined variable: %s\n", n->variable); //なければエラー
  exit(1);
  return 0;
}

void assignment(Node *n, FILE *fp)
{
  int offset = getOffset(n->child);
  fprintf(fp, "    sw $v0, %d($t0)\n", offset);
  fprintf(fp, "    nop\n");
  return ;
}

void OP(Node *n, FILE *fp)
{
  if (n->type == ADD_AST)
  {
    fprintf(fp, "    add $v0, $t1, $t3\n");
  }
  else if (n->type == SUB_AST)
  {
    fprintf(fp, "    sub $v0, $t1, $t3\n");
  }
  else if (n->type == MUL_AST)
  {
    fprintf(fp, "    mult $t1, $t3\n");
    fprintf(fp, "    mflo $v0\n");
  }
  else if (n->type == DIV_AST)
  {
    fprintf(fp, "    div $t1, $t3\n");
    fprintf(fp, "    mflo $v0\n");
  }
  return ;
}

int isCondOperator(Node *n)
{
  if (n == NULL)
  {
    return FALSE;
  }
  else if (n->type == EQ_AST || n->type == LT_AST || n->type == LTE_AST || n->type == GLT_AST || n->type == GLT_AST)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

void condition(Node *n, FILE *fp)
{
  printf("condition:n ");
  printNode(n);
  printf("condition:n->child ");
  printNode(n->child);
  printf("condition:n->child->brother ");
  printNode(n->child->brother);
  if (isCondOperator(n))
  {
    fprintf(fp,"# s left expression\n");
    printf("# s left expression\n");
    if (isIdentNumber(n->child))
    {
      genCodeNumberOrIdent(n->child, "t1", fp);
    }
    else
    {
      expression(n->child, fp);
      fprintf(fp, "    add $t1, $v0, $zero\n");
    }
    printf("# e left expression\n");
    fprintf(fp,"# e left expression\n");

    fprintf(fp,"# s right expression\n");
    printf("# s right expression\n");
    if(isIdentNumber(n->child->brother))
    {
      genCodeNumberOrIdent(n->child->brother, "t3", fp);
    }
    else
    {
      expression(n->child->brother, fp);
      fprintf(fp, "    add $t3, $v0, $zero\n");
    }
    fprintf(fp,"# e right expression\n");
    printf("# e right expression\n");
  }
  return ;
}

void branch(Node *n, FILE *fp, char *jump)
{
  if (n->type == EQ_AST)
  {
    fprintf(fp, "    bne $t1, $t3, %s\n", jump);
    fprintf(fp, "    nop\n");
  }
  else if (n->type == LT_AST)
  {
    fprintf(fp, "    slt $t2, $t1, $t3\n");
    fprintf(fp, "    beq $t2, $zero, %s\n", jump);
    fprintf(fp, "    nop\n");
  }
  else if (n->type == LTE_AST)
  {
    fprintf(fp, "    addi $t3, $t3, 1\n");
    fprintf(fp, "    slt $t2, $t1, $t3\n");
    fprintf(fp, "    beq $t2, $zero, %s\n", jump);
    fprintf(fp, "    nop\n");
  }
  else if (n->type == GT_AST)
  {
    fprintf(fp, "    slt $t2, $t3, $t1\n");
    fprintf(fp, "    beq $t2, $zero, %s\n", jump);
    fprintf(fp, "    nop\n");
  }
  else if (n->type == GLT_AST)
  {
    fprintf(fp, "    addi $t3, $t3, 1\n");
    fprintf(fp, "    slt $t2, $t3, $t1\n");
    fprintf(fp, "    beq $t2, $zero, %s\n", jump);
    fprintf(fp, "    nop\n");
  }
  return ;
}

void ifstmt(Node *n, FILE *fp)
{
  printf("ifstmt n:");
  printNode(n);
  printf("ifstmt n->child:");
  printNode(n->child);
  printf("ifstmt n->child->brother:");
  printNode(n->child->brother);

  int num = branches;
  branches++;

  condition(n->child, fp);

  char label[10];
  sprintf(label, "$IF%d", num);

  fprintf(fp,"# s if branch\n");
  branch(n->child, fp, label);
  fprintf(fp,"# e if branch\n");

  genCode(n->child->brother, fp);

  fprintf(fp, "   j $END%d\n", ifs);
  fprintf(fp, "$IF%d:\n", num);

  if(n->brother != NULL && n->brother->type == ELSE_STMT_AST){
    elsestmt(n->brother, fp);
  }
  return ;
}

void elsestmt(Node *n, FILE *fp)
{
  printf("else stmt :");
  printNode(n);
  if(n->child->type == STATEMENTS_AST)
  {
    genCode(n->child, fp);
  }
  else if(n->child->type == ELIF_STMT_AST)
  {
    ifstmt(n->child->child, fp);
    if(n->child->brother->type == ELSE_STMT_AST)
    {
      elsestmt(n->child->brother, fp);
    }
  }
  return ;
}

char *typeChar[] = {
    "IDENT_AST",
    "IDENTS_AST",
    "NUMBER_AST",
    "COND_AST",
    "GLT_AST",
    "LTE_AST",
    "LT_AST",
    "GT_AST",
    "EQ_AST",
    "ADD_AST",
    "DIV_AST",
    "MUL_AST",
    "SUB_AST",
    "DECRE_AST",
    "INCRE_AST",
    "OP_INCRE",
    "OP_DECRE",
    "ASSIGNMENT_STMT_AST",
    "ELSE_AST",
    "IF_STMT_AST",
    "ELIF_STMT_AST",
    "ELSE_STMT_AST",
    "FOR_LOOP_AST",
    "WHILE_LOOP_AST",
    "ARRAY_AST",
    "DEFINE_AST",
    "FUNCCALL_AST",
    "FUNC_AST",
    "PROGRAM_AST",
    "DECLARATIONS_AST",
    "DECL_STATEMENT_AST",
    "STATEMENTS_AST",
    "STATEMENT_AST",
    "ASSIGNMENT_AST",
    "EXPRESSION_AST",
    "TERM_AST",
    "FACTOR_AST",
    "ARGUMENT_DCLLIST_AST",
    "ARGUMENT_CALLLIST_AST",
    "FUNCTION_DCL_AST",
    "FUNCTION_CALL_AST",
    "ARRAY_INDEX_AST",
    "BREAK_AST",
    "CREMENT_AST",
    "VAR_AST",
    "LOOP_STMT_AST",
    "COND_STMT_AST",
    "DEFINE_ARRAY_AST",
    "OP_EQ",
    "OP_LT",
    "OP_GT",
    "OP_LTE",
    "OP_GLT",
    "OP_MUL",
    "OP_ADD",
    "OP_SUB",
    "OP_DIV",
    "IF_AST",
};

void printVars(int length)
{
  int i;
  for (i = 0; i < length; i++)
  {
    printf("variables[%d]\n", i);
    printVar(variables[i]);
    printf("\n");
  }
  return ;
}

void printVar(Var *var)
{
  printf("name: %s\n", var->name);
  printf("offset: %d\n", var->offset);
  printf("size: %d\n", var->size);
  printf("\n");
  return ;
}