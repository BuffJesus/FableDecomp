// Behaviour of map<CCharString, long>'s _M_create_node 0x00431773: a fresh
// 0x18-byte node whose value is copy-constructed from the argument (the key
// through CCharString's copy constructor, the long copied).
#include "fable_stlport_tree.h"
#include "fable_string.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef _STL::map<CCharString, long> Map;
typedef Map::_Rep_type Tree;
typedef Map::value_type Value;
typedef _STL::_Rb_tree_node<Value> Node;

static char g_log[64];
static const void* g_copyThis;
static const void* g_copyFrom;
CCharString::CCharString(const CCharString& other) { strcat(g_log, "copy;"); g_copyThis = this; g_copyFrom = &other; }

int main()
{
    static unsigned char treeStore[sizeof(Tree)];
    static unsigned char valueStore[sizeof(Value)];
    Tree& t = *reinterpret_cast<Tree*>(treeStore);
    Value& v = *reinterpret_cast<Value*>(valueStore);
    const_cast<long&>(v.second) = 0x1234;

    Node* n = t._M_create_node(v);
    if (n == 0 || strcmp(g_log, "copy;") != 0 || g_copyThis != &n->_M_value_field.first || g_copyFrom != &v.first ||
        n->_M_value_field.second != 0x1234 || (char*)&n->_M_value_field - (char*)n != 0x10)
    { printf("create: %s %p %p\n", g_log, (void*)n, g_copyThis); return 1; }
    free(n);

    // A second node is a separate allocation copied from its own argument.
    g_log[0] = 0;
    const_cast<long&>(v.second) = -7;
    Node* m = t._M_create_node(v);
    if (m == 0 || strcmp(g_log, "copy;") != 0 || g_copyThis != &m->_M_value_field.first || m->_M_value_field.second != -7)
    { printf("second: %s\n", g_log); return 1; }
    free(m);

    puts("FABLETLC_STLMAP_CCHARSTRING_LONG_CREATE_NODE_00431773_PASS");
    return 0;
}
