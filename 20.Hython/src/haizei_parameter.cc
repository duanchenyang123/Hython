#include <exception>
#include <haizei_parameter.h>
#include <haizei_type.h>
namespace haizei {
    Parameter::Parameter(SParameter next, int pos) : __next(next), __pos(pos) {}
    void Parameter::define_param(std::string name) {
        if (memory.find(name) != memory.end()) {
            throw std::runtime_error("redefined param : " + name);
        }
        this->memory[name] = haizei::null_val;
        return ;
    }
    int Parameter::pos() {return this->__pos;}
    SIValue Parameter::get(std::string name) const {
        if (memory.find(name) == memory.end()) {
            if (this->next() == nullptr) {
                throw std::runtime_error("undefined param : " + name);
            }
            return this->next()->get(name);
        }
        return this->memory[name];
    }
    void Parameter::set(std::string name, SIValue val) {
        if (memory.find(name) == memory.end()) {
            if (this->next() == nullptr) {
                throw std::runtime_error("undefined param : " + name);
            }
            this->next()->set(name, val);
            return ;
        }
        this->memory[name] = val;
        return ;
    }
    SParameter Parameter::next() const{
        return this->__next;
    }
        
}
