#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_string.h"
#include "fable_stlport_tree.h"
typedef _STL::map<CCharString, unsigned long>::_Rep_type BankStringTree;
typedef _STL::map<CCharString, unsigned long>::value_type BankStringValue;
typedef _STL::_Rb_tree_node<BankStringValue> BankStringNode;
static const CCharString* expected[4];
static unsigned destroyed;
CCharString::~CCharString() {
    if (destroyed >= 4 || this != expected[destroyed]) abort();
    ++destroyed;
}
static BankStringNode* node() {
    BankStringNode* p = static_cast<BankStringNode*>(malloc(sizeof(BankStringNode)));
    memset(p, 0, sizeof(*p)); return p;
}
int main() {
    unsigned char raw[sizeof(BankStringTree)];
    memset(raw, 0xA5, sizeof(raw));
    BankStringTree* tree = reinterpret_cast<BankStringTree*>(raw);
    tree->_M_erase(0);
    if (destroyed) return 1;
    BankStringNode* root = node();
    BankStringNode* left = node();
    BankStringNode* right = node();
    BankStringNode* rightLeft = node();
    root->_M_left = left; root->_M_right = right; right->_M_left = rightLeft;
    expected[0] = &right->_M_value_field.first;
    expected[1] = &rightLeft->_M_value_field.first;
    expected[2] = &root->_M_value_field.first;
    expected[3] = &left->_M_value_field.first;
    tree->_M_erase(root);
    if (destroyed != 4) return 2;
    for (unsigned i = 0; i < sizeof(raw); ++i) if (raw[i] != 0xA5) return 3;
    puts("STRING_TREE_ERASE_00579435_PASS");
    return 0;
}
