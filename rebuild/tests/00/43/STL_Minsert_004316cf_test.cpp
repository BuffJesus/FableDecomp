// Behaviour of map<CCharString, long>'s _M_insert 0x004316CF: the new node
// hangs left of __y when __y is the header (it becomes the root, leftmost and
// rightmost), when __x is set, or when its key sorts before __y's (unless __w
// is set); otherwise right. The leftmost/rightmost links follow, the tree is
// rebalanced (the real _Rb_global<bool>::_Rebalance) and the count goes up.
#include "fable_stlport_tree.h"
#include "fable_string.h"
#include <stdio.h>
#include <string.h>

typedef _STL::map<CCharString, long> Map;
typedef Map::_Rep_type Tree;
typedef Map::value_type Value;
typedef _STL::_Rb_tree_node_base Base;
typedef _STL::_Rb_tree_node<Value> Node;

// The landing calls Compare (0x00411570) instead of expanding it.
template long __fastcall CBasicString<char>::Compare(const char* left, const char* right);

struct Text { const char* text; };

static unsigned char g_newStore[sizeof(Node)];
static const Value* g_created;
static int g_creates;
template<> Node* Tree::_M_create_node(const Value& v)
{
    g_created = &v; ++g_creates;
    Node* n = reinterpret_cast<Node*>(g_newStore);
    n->_M_parent = n->_M_left = n->_M_right = (Base*)1;   // _M_insert must set all three
    return n;
}

static unsigned char g_treeStore[sizeof(Tree)];
static unsigned char g_nodeStore[2][sizeof(Node)];
static Text g_d = { "d" };
static Node* N(int i) { return reinterpret_cast<Node*>(g_nodeStore[i]); }
static Node* Z() { return reinterpret_cast<Node*>(g_newStore); }
static void SetKey(const void* key, Text* t) { *reinterpret_cast<Text**>(const_cast<void*>(key)) = t; }

static Tree& Build(int count)   // empty, or the single black root d
{
    Tree& t = *reinterpret_cast<Tree*>(g_treeStore);
    Base* h = N(0);
    *reinterpret_cast<Node**>(g_treeStore) = N(0);
    t._M_node_count = count;
    h->_M_color = _STL::_S_rb_tree_red;
    SetKey(&N(1)->_M_value_field.first, &g_d);
    N(1)->_M_color = _STL::_S_rb_tree_black;
    N(1)->_M_left = N(1)->_M_right = 0;
    if (count == 0)
    {
        h->_M_parent = 0; h->_M_left = h->_M_right = h;
    }
    else
    {
        h->_M_parent = N(1); h->_M_left = h->_M_right = N(1); N(1)->_M_parent = h;
    }
    return t;
}

static unsigned char g_valueStore[sizeof(Value)];
static Text g_key;

static int Run(const char* name, int count, const char* key, Base* x, Base* y, Base* w, bool wantLeft)
{
    Tree& t = Build(count);
    g_created = 0; g_creates = 0;
    g_key.text = key;
    Value& v = *reinterpret_cast<Value*>(g_valueStore);
    SetKey(&v.first, &g_key);
    Tree::iterator got = t._M_insert(x, y, v, w);
    Base* h = N(0); Base* z = Z();
    bool linked = wantLeft ? y->_M_left == z : y->_M_right == z;
    bool ends;
    if (y == h)
        ends = h->_M_parent == z && h->_M_left == z && h->_M_right == z && z->_M_color == _STL::_S_rb_tree_black;
    else if (wantLeft)
        ends = h->_M_left == z && h->_M_right == y && z->_M_color == _STL::_S_rb_tree_red;
    else
        ends = h->_M_left == y && h->_M_right == z && z->_M_color == _STL::_S_rb_tree_red;
    if (got._M_node != z || g_creates != 1 || g_created != &v || !linked || !ends || z->_M_parent != y ||
        z->_M_left != 0 || z->_M_right != 0 || t._M_node_count != (size_t)count + 1)
    {
        printf("%s: node %p creates %d linked %d ends %d count %u\n", name, (void*)got._M_node, g_creates, (int)linked,
               (int)ends, (unsigned)t._M_node_count);
        return 1;
    }
    return 0;
}

int main()
{
    Base* h = N(0); Base* d = N(1);
    int bad = 0;
    bad |= Run("empty", 0, "a", h, h, 0, true);
    bad |= Run("key first", 1, "b", 0, d, 0, true);
    bad |= Run("key after", 1, "f", 0, d, 0, false);
    bad |= Run("x set", 1, "f", d, d, 0, true);
    bad |= Run("w set", 1, "b", 0, d, d, false);
    if (bad) return 1;
    puts("FABLETLC_STLMAP_CCHARSTRING_LONG_M_INSERT_004316CF_PASS");
    return 0;
}