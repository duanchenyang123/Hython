/*************************************************************************
	> File Name: haizei_factory.h
	> Author: 
	> Mail: 
	> Created Time: 日  3/29 19:39:28 2020
 ************************************************************************/

#ifndef _HAIZEI_FACTORY_H
#define _HAIZEI_FACTORY_H

#include <haizei_master.h>

namespace haizei {

class PrintFactory : public IMaster::IFactory {
    virtual IMaster *create(ASTree *, SParameter );
};

class ExprFactory : public IMaster::IFactory {
    virtual IMaster *create(ASTree *, SParameter );
};

class BlockFactory : public IMaster::IFactory {
    virtual IMaster *create(ASTree *, SParameter );
};

class ConditionFactory : public IMaster::IFactory {
    virtual IMaster *create(ASTree *, SParameter );
};

class ControlFactory : public IMaster::IFactory {
    virtual IMaster *create(ASTree *, SParameter );
};


}

#endif
