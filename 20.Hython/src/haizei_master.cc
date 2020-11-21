#include <iostream>
#include <haizei_master.h>
#include<haizei_tree.h>
#include <haizei_runtime.h>
#include <haizei_visitor.h>
#include <haizei_parameter.h>
#include <haizei_util.h>

namespace haizei {
    IMaster::IMaster(ASTree *tree, SParameter p) : tree(tree), p(p) 
    {}
    IMaster::~IMaster() {}
    PrintMaster::PrintMaster(ASTree *tree, SParameter p) : IMaster(tree, p) {}
    ExprMaster::ExprMaster(ASTree *tree, SParameter p) : IMaster(tree, p) {}
    BlockMaster::BlockMaster(ASTree *tree, SParameter p) : IMaster(tree, p) {}
    ConditionMaster::ConditionMaster(ASTree *tree, SParameter p) : IMaster(tree, p) {}
    ControlMaster::ControlMaster(ASTree *tree, SParameter p) : IMaster(tree, p) {}

    void IMaster::IFactory::destory(IMaster *m) {
        delete m;
    }
    
    SIValue PrintMaster::run() {
        if (tree->type() != PRINT) {
            throw std::runtime_error("tree type is not print!");
        }
        for (int i = 0, I = tree->size(); i < I; i++) {
            auto child_tree = tree->at(i);
            SIValue ret = RunTimeEnv::GetValue(child_tree, p);
            ConvertStringVisitor vis;
            ret->accept(&vis);
            if (i) std::cout << " ";
            std::cout << vis.result;
        }
        std::cout << std::endl;
        return haizei::null_val;
    }
    
    SIValue ExprMaster::run() {
        switch (tree->type()) {
            case FUNC_CALL: {
                auto func = p->get(tree->at(0)->text());
                return func->run(p, tree->at(1));
            }
            case NOPE:{
                return haizei::True_val;
            }
            case INT: {
            	const char* s = this->tree->text().c_str();
            	int val_int;
				sscanf(s, "%d", &val_int);
                return std::make_shared<IntValue>(val_int);
        	}
            case STRING: {
                std::string tmp = ConvertStringToCString( this->tree->text());
                //字符串转译
                //hello\n -> hello换行符
                return std::make_shared<StringValue>(tmp.substr(1, tmp.size() - 2));
        	}
            case FLOAT: {
            	const char* s = this->tree->text().c_str();
            	double val_float;
				sscanf(s, "%lf", &val_float);
                return std::make_shared<FloatValue>(val_float);
        	}
        	case ID: {
            	return this->p->get(this->tree->text());//p是frame get方法传入string
        	}
 
            case PLUS: {
                SIValue a = RunTimeEnv::GetValue(tree->at(0), p);
                SIValue b = RunTimeEnv::GetValue(tree->at(1), p);
                return (*a) + (*b);
            }
            case MINUS: {
                SIValue a = RunTimeEnv::GetValue(tree->at(0), p);
                SIValue b = RunTimeEnv::GetValue(tree->at(1), p);
                return (*a) - (*b);
            }
            case TIMES: {
                SIValue a = RunTimeEnv::GetValue(tree->at(0), p);
                SIValue b = RunTimeEnv::GetValue(tree->at(1), p);
                return (*a) * (*b);
            }
            case MOD: {
                SIValue a = RunTimeEnv::GetValue(tree->at(0), p);
                SIValue b = RunTimeEnv::GetValue(tree->at(1), p);
                return (*a) % (*b);
            }
            case DIV: {
                SIValue a = RunTimeEnv::GetValue(tree->at(0), p);
                SIValue b = RunTimeEnv::GetValue(tree->at(1), p);
                return (*a) / (*b);
            }
            case ASSIGN: {
                std::string var = tree->at(0)->text();
                SIValue val = RunTimeEnv::GetValue(tree->at(1), p);
                p->set(var, val);
                return val;
            }
            case DEF: {
                for (int i = 0; i < tree->size(); i++){
                    std::string var = tree->at(i)->text();
                    p->define_param(var);
                    SIValue val = haizei::null_val;
                    if (tree->at(i)->size()) {
                        val = RunTimeEnv::GetValue(tree->at(i)->at(0), p);
                    }
                    p->set(var, val); 
                }
                return haizei::null_val;
            }
            default : {
                throw std::runtime_error("tree type is not expr!");

            }
        }
        return haizei::null_val;
    }
    
    SIValue BlockMaster::run() {
        if (tree->type() != BLOCK) {
            throw std::runtime_error("tree type is not block!");
        }
        p = std::make_shared<Parameter>(p);
        for (int i = 0, I = tree->size(); i < I; i++) {
            auto child_tree = tree->at(i);
            RunTimeEnv::GetValue(child_tree, p);
        }
        p = p->next();
        return haizei::null_val;
    }
    
    SIValue ConditionMaster::run() {
        switch (tree->type()) {
            case OR: {
                SIValue a = RunTimeEnv::GetValue(tree->at(0), p);
                if (a->isTrue()) return haizei::True_val;
                SIValue b = RunTimeEnv::GetValue(tree->at(1), p);
                if (b->isTrue()) return haizei::True_val;
                return haizei::False_val;
            }
            case AND: {
                SIValue a = RunTimeEnv::GetValue(tree->at(0), p);
                if (a->isFalse()) return haizei::False_val;
                SIValue b = RunTimeEnv::GetValue(tree->at(1), p);
                if (b->isTrue()) return haizei::True_val;
                return haizei::False_val;

            }
            case GE:{
                SIValue a = RunTimeEnv::GetValue(tree->at(0), p);
                SIValue b = RunTimeEnv::GetValue(tree->at(1), p);
                if ((*a) >= (*b)) return haizei::True_val;
                return haizei::False_val;
            }
            case GT: {
                SIValue a = RunTimeEnv::GetValue(tree->at(0), p);
                SIValue b = RunTimeEnv::GetValue(tree->at(1), p);
                if ((*a) > (*b)) return haizei::True_val;
                return haizei::False_val;
            }
            case LE: {
                SIValue a = RunTimeEnv::GetValue(tree->at(0), p);
                SIValue b = RunTimeEnv::GetValue(tree->at(1), p);
                if ((*a) <= (*b)) return haizei::True_val;
                return haizei::False_val;
            }
            case LITTLE: {
                SIValue a = RunTimeEnv::GetValue(tree->at(0), p);
                SIValue b = RunTimeEnv::GetValue(tree->at(1), p);
                if ((*a) < (*b)) return haizei::True_val;
                return haizei::False_val;
            }
            case EQ: {
                SIValue a = RunTimeEnv::GetValue(tree->at(0), p);
                SIValue b = RunTimeEnv::GetValue(tree->at(1), p);
                if ((*a) == (*b)) return haizei::True_val;
                return haizei::False_val;
            }
            case NE: {
                SIValue a = RunTimeEnv::GetValue(tree->at(0), p);
                SIValue b = RunTimeEnv::GetValue(tree->at(1), p);
                if ((*a) != (*b)) return haizei::True_val;
                return haizei::False_val;
            }
            default :{
                throw std::runtime_error("tree type is not block!");
            }
        }
        return haizei::False_val;
    }


    SIValue ControlMaster::run() {
       /* if (tree.type() != BLOCK) {
            throw std::runtime_error("tree type is not control!");
        }*/
        switch (tree->type()) {
            case IF:{
                SIValue cond_val = RunTimeEnv::GetValue(tree->at(0), p);
                if (cond_val->isTrue()) {
                    RunTimeEnv::GetValue(tree->at(1), p);
                } else if (tree->size() == 3) {
                    RunTimeEnv::GetValue(tree->at(2), p);
                }
                return haizei::null_val;
            }
            case FOR: {
                p = std::make_shared<Parameter>(p);
                RunTimeEnv::GetValue(tree->at(0), p);
                while (RunTimeEnv::GetValue(tree->at(1), p)->isTrue()) {
                    RunTimeEnv::GetValue(tree->at(3), p);
                    RunTimeEnv::GetValue(tree->at(2), p);
                }
                p = p->next();
                return haizei::null_val;

            }
            case WHILE: {
                while (RunTimeEnv::GetValue(tree->at(0), p)->isTrue()) {
                    RunTimeEnv::GetValue(tree->at(1), p);
                }
                return haizei::null_val;
            }
            case DOWHILE: {
                do {
                    RunTimeEnv::GetValue(tree->at(1), p);
                } while (RunTimeEnv::GetValue(tree->at(0), p)->isTrue());
                return haizei::null_val;
            }
            default: {
                throw std::runtime_error("tree type is not Control code!");

            }
        }


        return haizei::null_val;
    }
    




}


