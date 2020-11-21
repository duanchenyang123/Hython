/*************************************************************************
	> File Name: haizei_type.h
	> Author: 
	> Mail: 
	> Created Time: 日  3/29 18:16:07 2020
 ************************************************************************/

#ifndef _HAIZEI_TYPE_H
#define _HAIZEI_TYPE_H

#include <string>
#include <vector>
#include <haizei_shared_type.h>
namespace haizei {

class IValue {
public :
    IValue(std::string);
    class IVisitor {
    public :
        virtual void visit(IntValue *) = 0;
        virtual void visit(FloatValue *) = 0;
        virtual void visit(StringValue *) = 0;
        virtual void visit(FunctionValue *) = 0;
    };
    std::string type();
    virtual void accept(IVisitor *) = 0;
    virtual bool isTrue() = 0;
    virtual bool isFalse();
    virtual SIValue run(SParameter &, ASTree *);
    virtual SIValue operator+(IValue &);
    virtual SIValue operator-(IValue &);
    virtual SIValue operator*(IValue &);
    virtual SIValue operator/(IValue &);
    virtual SIValue operator%(IValue &);
    virtual bool operator<(IValue &);
    virtual bool operator<=(IValue &);
    virtual bool operator>(IValue &);
    virtual bool operator>=(IValue &);
    virtual bool operator==(IValue &);
    virtual bool operator!=(IValue &);
 	void operator_plus_error(IValue &);
    void operator_minus_error(IValue &);
    void operator_times_error(IValue &);
    void operator_div_error(IValue &);
    void operator_mod_error(IValue &);
    void operator_compare_error(IValue &);
    virtual ~IValue();
protected:
    std::string Tname;
};

class IntValue : public IValue {
public :
    IntValue(int = 0);
    virtual void accept(IVisitor *);
    virtual bool isTrue();
    virtual SIValue operator+(IValue &);
    virtual SIValue operator-(IValue &);
    virtual SIValue operator*(IValue &);
    virtual SIValue operator/(IValue &);
    virtual SIValue operator%(IValue &);
    virtual bool  operator<(IValue &);
    int val();
    ~IntValue();
private:
    int __val;
};
class FloatValue : public IValue {
public :
    FloatValue(double = 0.0);
    virtual void accept(IVisitor *);
    virtual bool isTrue();
    virtual SIValue operator+(IValue &);
    virtual SIValue operator-(IValue &);
    virtual SIValue operator*(IValue &);
    virtual SIValue operator/(IValue &);
    virtual bool  operator<(IValue &);
    double val();
    ~FloatValue();
private:
    double __val;
};
class StringValue : public IValue {
public :
    StringValue(std::string = "");
    virtual void accept(IVisitor *);
    virtual bool isTrue();
    virtual SIValue operator+(IValue &);
    virtual SIValue operator*(IValue &);
    virtual bool  operator<(IValue &);
    std::string val();
    ~StringValue();
private: 
    std::string __val;
};

class FunctionValue : public IValue {
public :
    FunctionValue(ASTree *, DFA *);
    virtual void accept(IVisitor *);
    virtual bool isTrue();
    virtual SIValue run(SParameter &, ASTree *);
    std::string val();
    std::string name();
    void set_init_param(SParameter);
    ~FunctionValue();
private:
    void convert(ASTree *);
    std::string func_name;
    std::vector<std::string> param;
    SParameter init_param;//初始作用域位置(函数定义之前)
    DFA *__val;//function相关定义
};

//new改成make_shared， IValue * 在上using后改成SIValue
static SIntValue null_val = std::make_shared<IntValue>(0);
static SIntValue True_val = std::make_shared<IntValue>(1);
static SIntValue False_val = std::make_shared<IntValue>(0);
static std::string ReturnValueName = "172567returnValue";

}
#endif
