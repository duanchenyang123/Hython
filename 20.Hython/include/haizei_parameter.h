/*************************************************************************
	> File Name: parameters.h
	> Author: 
	> Mail: 
	> Created Time: 日  3/29 16:28:05 2020
 ************************************************************************/

#ifndef _PARAMETER_H
#define _PARAMETER_H

#include <map>
#include <string>
#include <haizei_shared_type.h>
namespace haizei {

class Parameter {
public :
    Parameter(SParameter next, int = -1);
    void define_param(std::string);
    SIValue get(std::string) const;
    void set(std::string, SIValue );
    SParameter next() const;
    int pos();
private:
    mutable std::map<std::string, SIValue> memory;
    SParameter __next;
    int __pos;
};

}
#endif
