#include <testLexer.h>
#include <testParser.h>
#include <cassert>
#include <map>
#include <string>
#include <iostream>
#include <exception>

using std::map;
using std::string;
using std::cout;

class ExprTreeEvaluator {
    map<string,int> memory;
public:
    ExprTreeEvaluator(ExprTreeEvaluator *next) {
        this->next = next;
    }
    int find(string var) {
        if (this->memory.find(var) != this->memory.end()) {
            return this->memory[var];
        }
        if (this->next) return this->next->find(var);
        return 0;
    }
    int run(pANTLR3_BASE_TREE);
    ExprTreeEvaluator *next;
};

pANTLR3_BASE_TREE getChild(pANTLR3_BASE_TREE, unsigned);
const char* getText(pANTLR3_BASE_TREE tree);

int main(int argc, char* argv[])
{
  pANTLR3_INPUT_STREAM input;
  ptestLexer lex;
  pANTLR3_COMMON_TOKEN_STREAM tokens;
  ptestParser parser;

  assert(argc > 1);
  input = antlr3FileStreamNew((pANTLR3_UINT8)argv[1],ANTLR3_ENC_8BIT);
  lex = testLexerNew(input);
  tokens = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT,
                                            TOKENSOURCE(lex));
  parser = testParserNew(tokens);
  testParser_prog_return r = parser->prog(parser);

  pANTLR3_BASE_TREE tree = r.tree;

  ExprTreeEvaluator eval(NULL);
  int rr = eval.run(tree);
  cout << "Evaluator result: " << rr << '\n';

  parser->free(parser);
  tokens->free(tokens);
  lex->free(lex);
  input->close(input);

  return 0;
}

int ExprTreeEvaluator::run(pANTLR3_BASE_TREE tree) {
    pANTLR3_COMMON_TOKEN tok = tree->getToken(tree);
    if(tok) {
        switch(tok->type) {
        case BLOCK: {
            ExprTreeEvaluator new_node(this);
            int k = tree->getChildCount(tree);
            for (int i = 0; i < k; i++) {
                new_node.run(getChild(tree, i));
            }
            return 0;
        }
        case INT: {
            const char* s = getText(tree);
            if(s[0] == '~') {
                return -atoi(s+1);
            }
            else {
                return atoi(s);
            }
        }
        case ID: {
            string var(getText(tree));
            return this->find(var);
        }
        case PLUS:
            return run(getChild(tree,0)) + run(getChild(tree,1));
        case MINUS:
            return run(getChild(tree,0)) - run(getChild(tree,1));
        case TIMES:
            return run(getChild(tree,0)) * run(getChild(tree,1));
        case ASSIGN: {
            string var(getText(getChild(tree,0)));
            int val = run(getChild(tree,1));
            if (memory.find(var) == memory.end()) {
                throw std::runtime_error(string("undefined variable : ") + var);
            }
            memory[var] = val;
            return val;
        }
        case PRINT: {
            int k = tree->getChildCount(tree);
            for (int i = 0; i < k; i++) {
                pANTLR3_BASE_TREE child = getChild(tree, i);
                IValue *val = run(child);
                if (val -> intValue) dosomething;
                else if (val -> floatvalue) dosomething;
                if (i) printf(" ");
                printf("%d", val);
            }
            printf("\n");
            return 0;
        }
        case DEF: {
            int k = tree->getChildCount(tree);
            for (int i = 0; i < k; i++) {
                pANTLR3_BASE_TREE child = getChild(tree, i);
                string var(getText(child));
                if (memory.find(var) != memory.end()) {
                    throw std::runtime_error(string("redefined variable : ") + var);
                }
                int val = 0;
                if (child->getChildCount(child)) {
                    val = run(getChild(child, 0));
                }
                memory[var] = val;
            }
            return 0;
        }
        default:
            cout << "Unhandled token: #" << tok->type << '\n';
            return -1;
        }
    }
    else {
        cout << "in" << std::endl;
        int k = tree->getChildCount(tree);
        int r = 0;
        for (int i = 0; i < k; i++) {
            run(getChild(tree, i));
        }
        return 0;
    }
    return 0;
}

pANTLR3_BASE_TREE getChild(pANTLR3_BASE_TREE tree, unsigned i)
{
    assert(i < tree->getChildCount(tree));
    return (pANTLR3_BASE_TREE) tree->getChild(tree, i);
}

const char* getText(pANTLR3_BASE_TREE tree)
{
    return (const char*) tree->getText(tree)->chars;
}
