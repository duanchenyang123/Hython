grammar test;

options {
    language = C;
    output = AST;
    ASTLabelType=pANTLR3_BASE_TREE;
}

@header {
    #include <assert.h>
}

// The suffix '^' means make it a root.
// The suffix '!' means ignore it.

expr: multExpr ((PLUS^ | MINUS^) multExpr)*
    ;

PLUS: '+';
MINUS: '-';

multExpr 
    : atom (TIMES^ atom)*
    ;

TIMES: '*';

atom: INT
    | ID
    | '('! expr ')'!
    ;

ASSIGN: '=';
DEF: 'def';

var_def: ID^ (ASSIGN! expr)?;
def_expr: DEF^ var_def (','! var_def)* ';'!;

stmt: expr ';' NEWLINE -> expr // tree rewrite syntax
    | ID ASSIGN expr ';' NEWLINE -> ^(ASSIGN ID expr)
    | def_expr NEWLINE -> def_expr
    | print_expr NEWLINE -> print_expr
    | NEWLINE -> // ignore
    ;

BLOCK: '#';
PRINT: 'print';

expr_dot_s: expr (','! expr)*;
print_expr: PRINT expr_dot_s ';' -> ^(PRINT expr_dot_s);
block_code: stmt
    | block
    ;
block: '{' block_code* '}' -> ^(BLOCK block_code*);

prog
    : ( block {
            pANTLR3_STRING s = $block.tree->toStringTree($block.tree);
            assert(s->chars);
            printf("haizei tree \%s\n", s->chars);
       }
      )+
    ;

ID: ('a'..'z'|'A'..'Z')+ ;
INT: '~'? '0'..'9'+ ;
NEWLINE: '\r'? '\n' ;
WS : (' '|'\t')+ {$channel = HIDDEN;};
