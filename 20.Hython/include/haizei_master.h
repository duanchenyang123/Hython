/*************************************************************************
	> File Name: haizei_master.h
	> Author: 
	> Mail: 
	> Created Time: 日  3/29 19:39:40 2020
 ************************************************************************/

#ifndef _HAIZEI_MASTER_H
#define _HAIZEI_MASTER_H
#include <haizei_shared_type.h>

namespace haizei {

class IMaster {
public :
    IMaster(ASTree *, SParameter);
    class IFactory {
    public :
        virtual IMaster *create(ASTree *, SParameter) = 0;
        virtual void destory(IMaster *);
    };
    virtual SIValue run() = 0;
    virtual ~IMaster();

protected:
    ASTree* tree;
    SParameter p;
};

class PrintMaster: public IMaster{
public :
    PrintMaster(ASTree *, SParameter);
    virtual SIValue run();
private:

};

class ExprMaster: public IMaster {
public :
    ExprMaster(ASTree *, SParameter);
    virtual SIValue run();
private:
};

class BlockMaster :public IMaster{
public :
    BlockMaster(ASTree *, SParameter);
    virtual SIValue run();
private:
};

class ConditionMaster : public IMaster {
public:
    ConditionMaster(ASTree *, SParameter);
    virtual SIValue run();
};

class ControlMaster : public IMaster {
public:
    ControlMaster(ASTree *, SParameter);
    virtual SIValue run();
};













}

#endif
