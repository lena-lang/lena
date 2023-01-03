#include <stdio.h>
#include <stdlib.h>
#include "mpc.h"

// if compiling on Windows compile these functions
#ifdef _WIN32
#include <string.h>

static char buffer[2048];

// fake readline function 
char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char* cpy = malloc(strlen(buffer)+1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = '\0';
  return cpy;
}

// fake add_history function
void add_history(char* unused) {}

// otherwise include the editline headers
#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

int main(int argc, char** argv) {

// new parsers
mpc_parser_t* Number   = mpc_new("number");
mpc_parser_t* Operator = mpc_new("operator");
mpc_parser_t* Expr     = mpc_new("expr");
mpc_parser_t* Lena    = mpc_new("lena");

/* Define them with the following Language */
mpca_lang(MPCA_LANG_DEFAULT,
  "                                                     \
    number   : /-?[0-9]+/ ;                             \
    operator : '+' | '-' | '*' | '/' ;                  \
    expr     : <number> | '(' <operator> <expr>+ ')' ;  \
    lena    : /^/ <operator> <expr>+ /$/ ;             \
  ",
  Number, Operator, Expr, Lena);


// mpc_cleanup(4, Number, Operator, Expr, Lispy);

  puts("Lena Version v0.0.1");
  puts("Press Ctrl+c to Exit\n");

  while (1) {
    // now in either case readline will be correctly defined
    char* input = readline("lena> ");
    add_history(input);

    // attempt to parse the user input
    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Lena, &r)) {
      // on success print the AST
      mpc_ast_print(r.output);
      mpc_ast_delete(r.output);
    } else {
      // otherwise print the error
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }
    free(input);
  }

  return 0;
}