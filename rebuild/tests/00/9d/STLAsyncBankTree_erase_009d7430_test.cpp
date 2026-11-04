#include "fable_game_text_bank.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef _STL::set<CBankFileAsyncData*>::_Rep_type Tree;
typedef _STL::_Rb_tree_node<CBankFileAsyncData*> Node;
static Node* makeNode() {
    Node* n=static_cast<Node*>(malloc(sizeof(Node)));memset(n,0,sizeof(*n));
    n->_M_value_field=reinterpret_cast<CBankFileAsyncData*>(1);return n;
}
int main() {
    unsigned char raw[sizeof(Tree)];memset(raw,0xA5,sizeof(raw));
    Tree* tree=reinterpret_cast<Tree*>(raw);tree->_M_erase(0);
    Node* root=makeNode();root->_M_left=makeNode();root->_M_right=makeNode();root->_M_right->_M_left=makeNode();
    tree->_M_erase(root);
    for(unsigned i=0;i<sizeof(raw);++i)if(raw[i]!=0xA5)return 1;
    puts("ASYNC_TREE_ERASE_009d7430_PASS");return 0;
}
