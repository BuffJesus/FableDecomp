#include "fable_game_text_bank.h"

int main(int argc, char** argv)
{
    typedef _STL::set<CBankFileAsyncData*>::_Rep_type Tree;
    Tree* tree = reinterpret_cast<Tree*>(argv);
    Tree::iterator first(reinterpret_cast<_STL::_Rb_tree_node_base*>(argv));
    Tree::iterator last(reinterpret_cast<_STL::_Rb_tree_node_base*>(argv + argc));
    tree->erase(first, last);
    return argc;
}
