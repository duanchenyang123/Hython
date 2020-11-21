grammar hl;

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

defid_expr
    : DEF! defid (','! defid)* 
    ;
defid
    : ID^ (ASSIGN! expr)?
    ;

expr: multExpr ((PLUS^ | MINUS^) multExpr)*
    ;

multExpr
    : atom ((TIMES^ | DIV^ | MOD^) atom)*
    ;

atom: INT
    | ID
    | STRING
    | FLOAT
    | '('! expr ')'!
    | func_call
    ;

if_expr
    : IF^ '('! condition_expr ')'! stmt ( (ELSE) => ELSE! stmt )?
    ;

for_expr
    : FOR^ '('! init_expr ';'! condition_expr ';'! for_do_expr ')'! stmt
    ;

while_expr
    : WHILE^ '('! condition_expr ')'! stmt
    | DO block WHILE '(' condition_expr ')' ';' -> ^(DOWHILE condition_expr block)
    ;

init_expr
    : defid_expr -> ^(DEF defid_expr)
    | ID ASSIGN expr -> ^(ASSIGN ID expr)
    | -> ^(NOPE)
    ;

for_do_expr
    : ID ASSIGN expr -> ^(ASSIGN ID expr)
    | ->^(NOPE)
    ;

condition_expr: andExpr (OR^ andExpr)*
    ;
andExpr: cmp_atom (AND^ cmp_atom)*
    ;
cmp_atom: cond_atom ((GT^ | LITTLE^ | EQ^ | GE^ | LE^ | NE^) cond_atom)?
    ;
cond_atom
    : expr
    | -> ^(NOPE)
    ;

func_stmt:FUNCTION^ ID param block;
func_call: ID param_expr ->^(FUNC_CALL ID param_expr); 

param : '(' ')' -> ^(PARAM)
      |'(' def_param_expr ')' -> ^(PARAM def_param_expr)       
      ;
def_param_expr: ID(','! ID)*;


param_expr
    : '(' ')'->^(PARAM)
    | '(' param_sub_expr ')'->^(PARAM param_sub_expr)
    ;

param_sub_expr: condition_expr (','! condition_expr)*;



block: block_expr -> ^(BLOCK block_expr);

block_expr
    : '{'! (stmt)* '}'!
    ;

print_atom
    : condition_expr
    ;

expr_stmt
    : expr ';' -> expr //-> tree rewrite syntax
    | ID ASSIGN expr ';' -> ^(ASSIGN ID expr) //^ tree notation
    | PRINT^ print_atom (','! print_atom)* ';'!
    ;

control_stmt
    : for_expr
    | if_expr
    | while_expr
    | defid_expr ';' -> ^(DEF defid_expr)
    | block 
    | BREAK ';'!
    | CONTINUE ';'!
    | RETURN^ condition_expr ';'!
    ;

stmt: expr_stmt
    | control_stmt
    | func_stmt
    ;

prog
    : (stmt {
            pANTLR3_STRING s = $stmt.tree->toStringTree($stmt.tree);
            assert(s->chars);
            printf("tree \%s\n", s->chars);
    })+
    ;

MOD: '%';
DIV: '/';
DO: 'do';
DOWHILE: 'dowhile';
WHILE: 'while';
FOR: 'for';
PRINT: 'print';
IF: 'if';
ELSE: 'else';
OR: '||';
AND: '&&';
GE: '>=' | '=>';
NE: '!=';
LITTLE: '<';
LE: '<=' | '=<';
GT: '>';
EQ: '==';
NOPE: '000';
BREAK: 'break';
CONTINUE: 'continue';
FUNCTION:'function';
RETURN: 'return';
PARAM:'param';
FUNC_CALL:'func_call';

PLUS: '+';
MINUS: '-';
TIMES: '*';
DOT : ',';
ASSIGN: '=';
BLOCK: '{}';
DEF: 'def';

INT :	'-'? '0'..'9' + 
    ;
ID  :	('a'..'z'|'A'..'Z'|'_') ('a'..'z'|'A'..'Z'|'0'..'9'|'_')*
    ;

FLOAT
    :   ('0'..'9')+ '.' ('0'..'9')* EXPONENT?
    |   '.' ('0'..'9')+ EXPONENT?
    |   ('0'..'9')+ EXPONENT
    ;

COMMENT
    :   '//' ~('\n'|'\r')* '\r'? '\n' {$channel=HIDDEN;}
    |   '/*' ( options {greedy=false;} : . )* '*/' {$channel=HIDDEN;}
    ;

WS  :   ( ' '
        | '\t'
        | '\r'
        | '\n'
        ) {$channel=HIDDEN;}
    ;

STRING
    :  '"' ( ESC_SEQ | ~('\\'|'"') )* '"'
    ;

fragment
EXPONENT : ('e'|'E') ('+'|'-')? ('0'..'9')+ ;

fragment
HEX_DIGIT : ('0'..'9'|'a'..'f'|'A'..'F') ;

fragment
ESC_SEQ
    :   '\\' ('b'|'t'|'n'|'f'|'r'|'\"'|'\''|'\\')
    |   UNICODE_ESC
    |   OCTAL_ESC
    ;

fragment
OCTAL_ESC
    :   '\\' ('0'..'3') ('0'..'7') ('0'..'7')
    |   '\\' ('0'..'7') ('0'..'7')
    |   '\\' ('0'..'7')
    ;

fragment
UNICODE_ESC
    :   '\\' 'u' HEX_DIGIT HEX_DIGIT HEX_DIGIT HEX_DIGIT
    ;
