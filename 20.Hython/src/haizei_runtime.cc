#include <haizei_runtime.h>
#include <haizei_parameter.h>
#include <haizei_handler.h>
#include <haizei_master.h>
#include <haizei_tree.h>
#include <haizei_dfa.h>
namespace haizei {
    RunTimeEnv::RunTimeEnv(ASTree *tree) : tree(tree) {}
    void RunTimeEnv::run() {
        SParameter p = std::make_shared<Parameter>(nullptr);
        FactoryHandler::init_factory(tree);
        DFA program(tree);//初始化自动机结构
        program.run(p);
        return ;
    }
    SIValue RunTimeEnv::GetValue(ASTree *tree, SParameter p) {
        IMaster *m = tree->factory->create(tree, p);//根据树的节点创建具体工厂对应的具体经理
        SIValue val = m->run();
        tree->factory->destory(m);
        return val;
    }
    
    
}

