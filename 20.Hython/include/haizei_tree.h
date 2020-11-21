/*************************************************************************
	> File Name: ASTree.h
	> Author: 
	> Mail: 
	> Created Time: 日  3/29 15:36:27 2020
 ************************************************************************/

#ifndef _ASTREE_H
#define _ASTREE_H
#include <string>
#include <hlLexer.h>
#include <hlParser.h>
#include <haizei_master.h>
#include <vector>
namespace haizei {
    
class ASTree {
public :
    ASTree(pANTLR3_BASE_TREE tree);
    int size() const;
    ASTree *at(int ind) ;
    std::string text() const;
    int type() const;
    IMaster::IFactory *factory;//任意工厂类型指针 没有被初始化 都是空 应该交给运行环境的run方法去赋值

private:
    pANTLR3_BASE_TREE tree;
    int __type;
    int __size;
    std::vector<ASTree *> child;
};

}
#endif
