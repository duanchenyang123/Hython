#include <iostream>
#include <sstream>
#include <haizei_type.h>
#include <stdexcept>
#include <haizei_visitor.h>
#include <haizei_dfa.h>
#include <haizei_tree.h>
#include <haizei_parameter.h>
#include <haizei_runtime.h>

namespace haizei {
    IValue::IValue(std::string Tname) : Tname(Tname) {};
    std::string IValue::type() {return this->Tname; } 
    IValue::~IValue() {}
    IntValue::~IntValue() {/*std::cout << "destrouctor : " << val() << std::endl;*/}
    FloatValue::~FloatValue() {/*std::cout << "destrouctor : " << val() << std::endl;*/}
    StringValue::~StringValue() {/*std::cout << "destrouctor : " << val() << std::endl;*/}
    FunctionValue::~FunctionValue() {/*std::cout << "destrouctor : " << val() << std::endl;*/}
    
    // visitor function
    void IntValue::accept(IVisitor *vis) { 
        vis->visit(this); 
    }
    void FloatValue::accept(IVisitor *vis) { 
        vis->visit(this); 
    }
    void StringValue::accept(IVisitor *vis) { 
        vis->visit(this); 
    }

    void FunctionValue::accept(IVisitor *vis) {
        vis->visit(this);
    }

    // convert constructor
    IntValue::IntValue(int x ) : IValue("int"),__val(x) {}
    FloatValue::FloatValue(double x  ) : IValue("float"),__val(x) {}
    StringValue::StringValue(std::string x ) :IValue("string"), __val(x) {}
    FunctionValue::FunctionValue(ASTree *tree, DFA *dfa) :
        IValue("function"), __val(dfa) {convert(tree);
    }

    // get value function
    int IntValue::val() { return __val; }
    double FloatValue::val() { return __val; }
    std::string StringValue::val() { return __val; }
//IValue operator
    bool IValue::isFalse() {
        return !(this->isTrue());
    }

    SIValue IValue::run(SParameter &p, ASTree *tree) {
        std::string msg = this->type() + " is not callable!";
        throw std::runtime_error(msg);
        return haizei::null_val;
    }

    SIValue IValue::operator+(IValue &obj) {
   		this->operator_plus_error(obj);
        return haizei::null_val;
    }
    
    SIValue IValue::operator-(IValue &obj) {
 		this->operator_minus_error(obj);
        return haizei::null_val;
    }

    SIValue IValue::operator*(IValue &obj) {
        this->operator_times_error(obj);
        return haizei::null_val;
    }
	SIValue IValue::operator/(IValue &obj) {
        this->operator_div_error(obj);
        return haizei::null_val;
    }
    SIValue IValue::operator%(IValue &obj) {
        this->operator_mod_error(obj);
        return haizei::null_val;
    }
    
    bool IValue::operator<(IValue &obj) {
        this->IValue::operator_compare_error(obj);
        return false;
    }

    bool IValue::operator>(IValue &obj) {
        return obj < (*this);    
    }
    bool IValue::operator<=(IValue &obj) {
        return !(obj < (*this));    
    }
    bool IValue::operator>=(IValue &obj) {
        return !((*this) < obj);    
    }
    bool IValue::operator==(IValue &obj) {
        return !((*this) != obj);
    }
    bool IValue::operator!=(IValue &obj) {
        return (obj < (*this) || (*this) < obj);    
    }



    void IValue::operator_plus_error(IValue &obj) {
        std::string msg = "\nnot support operator (";
        msg += this->type() + " + " + obj.type() + ")";
        throw std::runtime_error(msg);
        return ; 
    }
    
    void IValue::operator_minus_error(IValue &obj) {
        std::string msg = "\nnot support operator (";
        msg += this->type() + " - " + obj.type() + ")";
        throw std::runtime_error(msg);
        return ; 
    }

    void IValue::operator_times_error(IValue &obj) {
        std::string msg = "\nnot support operator (";
        msg += this->type() + " * " + obj.type() + ")";
        throw std::runtime_error(msg);
        return ;
    }
 
    void IValue::operator_div_error(IValue &obj) {
        std::string msg = "\nnot support operator (";
        msg += this->type() + " / " + obj.type() + ")";
        throw std::runtime_error(msg);
        return ;
    }
 
    void IValue::operator_mod_error(IValue &obj) {
        std::string msg = "\nnot support operator (";
        msg += this->type() + " % " + obj.type() + ")";
        throw std::runtime_error(msg);
        return ;
    }
        
    void IValue::operator_compare_error(IValue &obj) {
        std::string msg = "\n(" + this->type() + " vs " + obj.type();
        msg += ") can't be compared!";
        throw std::runtime_error(msg);
        return ;
    }

//int start
    bool IntValue::isTrue() {
        return this->val() != 0;
    }
    SIValue IntValue::operator+(IValue &obj) {
        IntValuePlusOpVisitor vis(this);
        obj.accept(&vis);//accept会调用visit方法
        return vis.result;
    }

    SIValue IntValue::operator-(IValue &obj) {
        IntValueMinusOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result;
        
    }

    SIValue IntValue::operator*(IValue &obj) {
        IntValueTimesOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result;
        
    }
    SIValue IntValue::operator/(IValue &obj) {
        IntValueDivOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result;
        
    }
    SIValue IntValue::operator%(IValue &obj) {
        IntValueModOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result;
        
    }
    bool IntValue::operator<(IValue &obj) {
        IntValueLittleOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result->isTrue();
    }
//int end

//float start
    bool FloatValue::isTrue() {
        return this->val() != 0.0;
    }
    SIValue FloatValue::operator+(IValue &obj) {
        FloatValuePlusOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result;
    }

    SIValue FloatValue::operator-(IValue &obj) {
        FloatValueMinusOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result;
        
    }

    SIValue FloatValue::operator*(IValue &obj) {
        FloatValueTimesOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result;
        
    }
    SIValue FloatValue::operator/(IValue &obj) {
        FloatValueDivOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result;
        
    }
    bool FloatValue::operator<(IValue &obj) {
        FloatValueLittleOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result->isTrue();
    }
//float end    

//string start
    bool StringValue::isTrue() {
        return this->val() != "";
    }
    SIValue StringValue::operator+(IValue &obj) {
        StringValuePlusOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result;
        
    }
    SIValue StringValue::operator*(IValue &obj) {
        StringValueTimesOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result;
        
    }
    bool StringValue::operator<(IValue &obj) {
        StringValueLittleOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result->isTrue();
    }
//string end

    
 //Fuction start
    bool FunctionValue::isTrue() {
        return true;
    }

    std::string FunctionValue::val() {
        std::string ret = "function " + this->func_name;
        ret += "(";
        for (int i = 0; i < this->param.size(); i++) {
            if (i) ret += ", ";
            ret += this->param[i];
        }
        ret += ");";
        return ret;
    }
    std::string FunctionValue::name() {
        return this->func_name;
    } 
    void FunctionValue::convert(ASTree *tree) {
        this->func_name = tree->at(0)->text();
        this->param.clear();
        for (int i = 0, I = tree->at(1)->size(); i < I; i++) {
            this->param.push_back(tree->at(1)->at(i)->text());
        }
        return ;
    }
    
    void FunctionValue::set_init_param(SParameter init_param){
        this->init_param = init_param;
        return ;
    }

    SIValue FunctionValue::run(SParameter &p, ASTree *param) {
        this->init_param = std::make_shared<Parameter>(this->init_param);
        //std::cout << "add param done" <<std::endl;
        if (param->size() != this->param.size()) {
            //传入参数数量不等于函数形参数量
            std::string msg;
            std::stringstream out(msg);
            out << this->name() << " call is wrong" << std::endl;
            out << "\texcept " << this->param.size() <<" params, but ";
            out<< param->size() <<" given." << std::endl;
            throw std::runtime_error(msg);
            return haizei::null_val;
        }
        for (int i = 0; i < param->size(); i++) {
            this->init_param->define_param(this->param[i]);
            this->init_param->set(
                this->param[i], 
                RunTimeEnv::GetValue(param->at(i), p->next())
            );
        }
        //std::cout<< "add param value done" << std::endl;
        this->init_param->define_param(haizei::ReturnValueName);
        this->init_param->set(haizei::ReturnValueName, haizei::null_val);
        //std::cout<< "run function done" << std::endl;
        this->__val->run(this->init_param);
        SIValue retValue = this->init_param->get(haizei::ReturnValueName); 
        //std::cout<< "get return value done" << std::endl;
        this->init_param = this->init_param->next();
        return retValue;
    }


}
