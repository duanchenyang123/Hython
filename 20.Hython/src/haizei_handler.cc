#include <stdexcept>
#include<string>
#include <haizei_handler.h>
#include <haizei_tree.h>
#include<haizei_factory.h>
namespace haizei{
    FactoryHandler* FactoryHandler::get() {
        if (head == nullptr) {
            head = new PrintFactoryHandler();
            head->next = new ExprFactoryHandler();
            head->next->next = new BlockFactoryHandler();
            head->next->next->next = new ConditionFactoryHandler();
            head->next->next->next->next = new ControlFactoryHandler();
        }
        return head;
    }
    void FactoryHandler::init_factory(ASTree *tree) {
        auto p = FactoryHandler::get();
        for (; p != nullptr; p = p->next) {
            if (!p->is_valid(tree)) continue;
            p->do_factory(tree);
            break;
        }
        if (p == nullptr) {
            char str[100];
            sprintf(str, "%d", tree->type());
            throw std::runtime_error(std::string("there is no handler with ") + str);
        }
        for (int i = 0; i < tree->size(); i++) {
            FactoryHandler::init_factory(tree->at(i));
        }
        return ;
    }

    bool PrintFactoryHandler::is_valid(ASTree *tree) {
        switch (tree->type()) {
            case PRINT: return true;
            default : return false;
        }
        return false;
    }
    
    void PrintFactoryHandler::do_factory(ASTree *tree) {
        tree->factory = factory;
        return ;
    }

    bool ExprFactoryHandler::is_valid(ASTree *tree) {
        switch (tree->type()) {
            case FUNC_CALL:
            case NOPE:
            case INT:
            case STRING:
            case FLOAT:
            case ID:
            case PLUS:
            case MINUS:
            case TIMES:
            case DIV:
            case MOD:
            case ASSIGN: 
            case DEF:
                return true;
            default:
                return false;
        }
    }

    void ExprFactoryHandler::do_factory(ASTree *tree) {
        tree->factory = factory;
        return ;
    }

    bool BlockFactoryHandler::is_valid(ASTree *tree) {
        switch(tree->type()) {
            case BLOCK: return true;
            default : return false;
        }
        return false;
    }

    void BlockFactoryHandler::do_factory(ASTree *tree) {
        tree->factory = factory;
        return ;
    }

    bool ConditionFactoryHandler::is_valid(ASTree *tree) {
        switch(tree->type()) {
            case AND: 
            case OR: 
            case GE: 
            case GT: 
            case LE:
            case LITTLE: 
            case EQ: 
            case NE: return true;
            default : return false;
        }
        return false;
    }

    void ConditionFactoryHandler::do_factory(ASTree *tree) {
        tree->factory = factory;
        return ;
    }

    bool ControlFactoryHandler::is_valid(ASTree *tree) {
        switch(tree->type()) {
            case RETURN:
            case PARAM:
            case FUNCTION:
            case BREAK:
            case CONTINUE:
            case IF: 
            case FOR: 
            case WHILE: 
            case DOWHILE: return true;
            default : return false;
        }
        return false;
    }
  
    
    void ControlFactoryHandler::do_factory(ASTree *tree) {
        tree->factory = factory;
        return ;
    }

    BlockFactory *BlockFactoryHandler::factory = new BlockFactory();
    ExprFactory *ExprFactoryHandler::factory = new ExprFactory();
    PrintFactory *PrintFactoryHandler::factory = new PrintFactory();
    ConditionFactory *ConditionFactoryHandler::factory = new ConditionFactory();
    ControlFactory *ControlFactoryHandler::factory = new ControlFactory();
    FactoryHandler *FactoryHandler::head = nullptr;


}
