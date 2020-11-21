#include <stdexcept>
#include <haizei_tree.h>

namespace haizei {
    ASTree::ASTree(pANTLR3_BASE_TREE tree) : tree(tree) {
        pANTLR3_COMMON_TOKEN tok = tree->getToken(tree);
        __type = tok->type;
        __size = tree->getChildCount(tree);
        for (int i = 0; i < __size; i++) {
            this->child.push_back(new ASTree((pANTLR3_BASE_TREE)(tree->getChild(tree, i))));
        }
    }
    int ASTree::type() const { return this->__type; }
    int ASTree::size() const {
        return __size;
    }
    ASTree *ASTree::at(int ind)  {
        if (size() <= ind) {
            throw std::runtime_error("AST error, there is no enough nodes!");
        }
        return this->child[ind];
    }
    std::string ASTree::text() const {
        return (const char*) tree->getText(tree)->chars;
    }
}
