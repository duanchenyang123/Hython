/*************************************************************************
	> File Name: haizei_runtime.h
	> Author: 
	> Mail: 
	> Created Time: 日  3/29 19:10:33 2020
 ************************************************************************/

#ifndef _HAIZEI_RUNTIME_H
#define _HAIZEI_RUNTIME_H
#include <haizei_shared_type.h>
namespace haizei{

class RunTimeEnv {
public :
    RunTimeEnv(ASTree *);
    void run();
    static SIValue GetValue(ASTree *, SParameter);

private:
    ASTree* tree;
};

}
#endif
