#include <haizei_factory.h>
#include <haizei_tree.h>
#include <haizei_parameter.h>
#include <haizei_master.h>
namespace haizei {
    IMaster *PrintFactory::create(ASTree *tree, SParameter p) {
        return new PrintMaster(tree, p);
    }
    
    IMaster *ExprFactory::create(ASTree *tree, SParameter p) {
        return new ExprMaster(tree, p);
    }

    IMaster *BlockFactory::create(ASTree *tree, SParameter p) {
        return new BlockMaster(tree, p);
    }

    IMaster *ConditionFactory::create(ASTree *tree, SParameter p) {
        return new ConditionMaster(tree, p);
    }
    
    IMaster *ControlFactory::create(ASTree *tree, SParameter p) {
        return new ControlMaster(tree, p);
    }
}
