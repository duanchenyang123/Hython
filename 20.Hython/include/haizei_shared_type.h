/*************************************************************************
	> File Name: include/haizei_shared_type.h
	> Author: 
	> Mail: 
	> Created Time: 2020年11月16日 星期一 21时45分24秒
 ************************************************************************/

#ifndef HAIZEI_SHARED_TYPE_H
#define HAIZEI_SHARED_TYPE_H
#include <memory>

namespace haizei{

class ASTree;
class Parameter;
class IValue;
class IntValue;
class FloatValue;
class StringValue;
class DFA;
class IDFANode;
class FunctionValue;

using SIValue = std::shared_ptr<IValue>;
using SIntValue = std::shared_ptr<IntValue>;
using SFloatValue = std::shared_ptr<FloatValue>;
using SStringValue = std::shared_ptr<StringValue>;
using SParameter = std::shared_ptr<Parameter>;
using SFunctionValue = std::shared_ptr<FunctionValue>;

}




#endif
