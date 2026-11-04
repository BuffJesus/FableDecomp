// Behaviour of the console-input map's unhinted insert_unique 0x009EFB80 on a
// real three-node tree (b, d, f; header red, root black): a new key links under
// the leaf it reaches (_M_insert with STLport's arguments, result .second
// true); an equal key returns its node with .second false.
#include "fable_console.h"
#include "fable_string.h"
#include <stdio.h>
#include <string.h>

typedef _STL::map<CCharString, CCountedPointer<CConsoleInputBase> > Map;
typedef Map::_Rep_type Tree;
typedef Map::value_type Value;
typedef _STL::_Rb_tree_node_base Base;
typedef _STL::_Rb_tree_node<Value> Node;

// Supply the comparison dependency; insertion remains candidate-linked.
template long __fastcall CBasicString<char>::Compare(const char* left, const char* right);

// A string is its data pointer; the data's leading buffer pointer is the text.
struct Text { const char* text; };

static char g_log[128];
static Base* g_args[3];
static const Value* g_arg_v;
static unsigned char g_new[sizeof(Node)];

template<> Tree::iterator Tree::_M_insert(_Base_ptr x, _Base_ptr y, const Value& v, _Base_ptr w)
{
    strcat(g_log, "insert;");
    g_args[0] = x; g_args[1] = y; g_args[2] = w; g_arg_v = &v;
    return iterator(reinterpret_cast<Base*>(g_new));
}

static unsigned char g_treeStore[sizeof(Tree)];
static unsigned char g_nodeStore[4][sizeof(Node)];
static Text g_text[4] = { { "" }, { "b" }, { "d" }, { "f" } };
static Node* N(int i) { return reinterpret_cast<Node*>(g_nodeStore[i]); }
static void SetKey(const void* key, Text* t) { *reinterpret_cast<Text**>(const_cast<void*>(key)) = t; }

static Tree& Build(int count)
{
    Tree& t = *reinterpret_cast<Tree*>(g_treeStore);
    Base* h = N(0);
    *reinterpret_cast<Node**>(g_treeStore) = N(0);
    t._M_node_count = count;
    h->_M_color = _STL::_S_rb_tree_red;
    for (int i = 1; i <= 3; ++i)
    {
        SetKey(&N(i)->_M_value_field.first, &g_text[i]);
        N(i)->_M_color = _STL::_S_rb_tree_black;
        N(i)->_M_left = N(i)->_M_right = 0;
    }
    if (count == 0)
    {
        h->_M_parent = 0; h->_M_left = h->_M_right = h;
    }
    else                   // d with children b and f
    {
        h->_M_parent = N(2); h->_M_left = N(1); h->_M_right = N(3);
        N(2)->_M_parent = h; N(2)->_M_left = N(1); N(2)->_M_right = N(3);
        N(1)->_M_parent = N(2); N(3)->_M_parent = N(2);
        N(1)->_M_color = N(3)->_M_color = _STL::_S_rb_tree_red;
    }
    return t;
}

static unsigned char g_valueStore[sizeof(Value)];
static Text g_key;

static int Run(const char* name, int count, const char* key, const char* wantLog, const void* wantNode,
               bool wantInserted, Base* x, Base* y)
{
    Tree& t = Build(count);
    g_log[0] = 0; g_args[0] = g_args[1] = g_args[2] = (Base*)1; g_arg_v = 0;
    g_key.text = key;
    Value& v = *reinterpret_cast<Value*>(g_valueStore);
    SetKey(&v.first, &g_key);
    _STL::pair<Tree::iterator, bool> got = t.insert_unique(v);
    if (strcmp(g_log, wantLog) != 0 || got.first._M_node != wantNode || got.second != wantInserted ||
        (wantInserted && (g_args[0] != x || g_args[1] != y || g_args[2] != 0 || g_arg_v != &v)))
    {
        printf("%s: log '%s' node %p %d args %p %p %p\n", name, g_log, (void*)got.first._M_node, (int)got.second,
               (void*)g_args[0], (void*)g_args[1], (void*)g_args[2]);
        return 1;
    }
    return 0;
}

int main()
{
    Base* h = N(0); Base* b = N(1); Base* d = N(2); Base* f = N(3);
    const void* nw = g_new;
    int bad = 0;
    bad |= Run("empty", 0, "a", "insert;", nw, true, h, h);
    bad |= Run("before first", 3, "a", "insert;", nw, true, b, b);
    bad |= Run("after b", 3, "c", "insert;", nw, true, 0, b);
    bad |= Run("equal b", 3, "b", "", b, false, 0, 0);
    bad |= Run("equal d", 3, "d", "", d, false, 0, 0);
    bad |= Run("before f", 3, "e", "insert;", nw, true, 0, f);
    bad |= Run("after last", 3, "g", "insert;", nw, true, 0, f);
    bad |= Run("equal f", 3, "f", "", f, false, 0, 0);
    if (bad) return 1;
    puts("INPUT_INSERT_PASS");
    return 0;
}