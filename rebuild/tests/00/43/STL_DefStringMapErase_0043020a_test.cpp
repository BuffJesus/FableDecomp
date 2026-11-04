#include "fable_def_string.h"
#include <stdlib.h>
#include <stdio.h>
#include <new>

typedef _STL::pair<const CCharString,long> Entry;
typedef _STL::_Rb_tree<CCharString,Entry,_STL::_Select1st<Entry>,_STL::less<CCharString> > Tree;
typedef _STL::_Rb_tree_node<Entry> Node;
static unsigned count;
static const CCharString* destroyed[4];
CCharString::CCharString():storage_(0) {}
CCharString::~CCharString() { destroyed[count++]=this; }
static Node* node() {
    Node* p=(Node*)calloc(1,sizeof(Node));
    new (const_cast<CCharString*>(&p->_M_value_field.first)) CCharString;
    return p;
}
int main() {
    Tree tree;
    tree._M_erase(0);
    if(count) return 1;
    Node* root=node(); Node* left=node(); Node* right=node(); Node* deep=node();
    root->_M_left=left;root->_M_right=right;right->_M_right=deep;
    const CCharString* expected[]={&deep->_M_value_field.first,&right->_M_value_field.first,&root->_M_value_field.first,&left->_M_value_field.first};
    tree._M_erase(root);
    if(count!=4) return 2;
    for(unsigned i=0;i<4;++i) if(destroyed[i]!=expected[i]) return 3;
    free(tree._M_header._M_data);
    puts("DEFSTRING_MAP_ERASE_OK cases=2");return 0;
}
