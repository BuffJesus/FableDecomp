// Behaviour of map<CCharString, long>'s hinted insert_unique 0x00516AF0 on a
// real three-node tree (b, d, f; header red, root black): a hint at begin(),
// end() or a middle node inserts next to it when the key belongs there
// (_M_insert with STLport's arguments), returns the hint for an equal key, and
// otherwise falls back on the unhinted insert_unique (its .first).
#include "fable_stlport_tree.h"
#include "fable_string.h"
#include <stdio.h>
#include <string.h>

typedef _STL::map<CCharString, long> Map;
typedef Map::_Rep_type Tree;
typedef Map::value_type Value;
typedef _STL::_Rb_tree_node_base Base;
typedef _STL::_Rb_tree_node<Value> Node;

// A string is its data pointer; the data's leading buffer pointer is the text.
struct Text { const char* text; };

static char g_log[128];
static Base* g_args[3];
static const Value* g_arg_v;
static unsigned char g_new[sizeof(Node)];
static unsigned char g_unhinted[sizeof(Node)];

template<> Tree::iterator Tree::_M_insert(_Base_ptr x, _Base_ptr y, const Value& v, _Base_ptr w)
{
    strcat(g_log, "insert;");
    g_args[0] = x; g_args[1] = y; g_args[2] = w; g_arg_v = &v;
    return iterator(reinterpret_cast<Base*>(g_new));
}

template<> _STL::pair<Tree::iterator, bool> Tree::insert_unique(const Value& v)
{
    strcat(g_log, "unhinted;");
    g_arg_v = &v;
    return _STL::pair<iterator, bool>(iterator(reinterpret_cast<Base*>(g_unhinted)), true);
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
    else if (count == 1)   // just d
    {
        h->_M_parent = N(2); h->_M_left = h->_M_right = N(2); N(2)->_M_parent = h;
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

static int Run(const char* name, int count, Base* hint, const char* key, const char* wantLog,
               const void* wantResult, Base* x, Base* y, Base* w)
{
    Tree& t = Build(count);
    g_log[0] = 0; g_args[0] = g_args[1] = g_args[2] = (Base*)1; g_arg_v = 0;
    g_key.text = key;
    Value& v = *reinterpret_cast<Value*>(g_valueStore);
    SetKey(&v.first, &g_key);
    Tree::iterator got = t.insert_unique(Tree::iterator(hint), v);
    if (strcmp(g_log, wantLog) != 0 || got._M_node != wantResult ||
        (strcmp(wantLog, "insert;") == 0 && (g_args[0] != x || g_args[1] != y || g_args[2] != w || g_arg_v != &v)) ||
        (strcmp(wantLog, "unhinted;") == 0 && g_arg_v != &v))
    {
        printf("%s: log '%s' result %p args %p %p %p\n", name, g_log, (void*)got._M_node,
               (void*)g_args[0], (void*)g_args[1], (void*)g_args[2]);
        return 1;
    }
    return 0;
}

int main()
{
    Base* h = N(0); Base* b = N(1); Base* d = N(2); Base* f = N(3);
    const void* nw = g_new; const void* un = g_unhinted;
    int bad = 0;
    // Empty tree (begin() == end()): the unhinted insert.
    bad |= Run("empty", 0, h, "a", "unhinted;", un, 0, 0, 0);
    // Hint begin() (b).
    bad |= Run("begin before", 3, b, "a", "insert;", nw, b, b, 0);
    bad |= Run("begin equal", 3, b, "b", "", b, 0, 0, 0);
    bad |= Run("begin after, b has no right", 3, b, "c", "insert;", nw, 0, b, b);
    bad |= Run("begin far", 3, b, "e", "unhinted;", un, 0, 0, 0);
    bad |= Run("single, after it", 1, d, "e", "insert;", nw, 0, d, d);
    bad |= Run("single, before it", 1, d, "c", "insert;", nw, d, d, 0);
    // Hint end().
    bad |= Run("end after last", 3, h, "g", "insert;", nw, 0, f, h);
    bad |= Run("end not after", 3, h, "e", "unhinted;", un, 0, 0, 0);
    bad |= Run("end equal last", 3, h, "f", "unhinted;", un, 0, 0, 0);
    // Hint a middle node (d; before = b, after = f).
    bad |= Run("mid between b and d", 3, d, "c", "insert;", nw, 0, b, b);
    bad |= Run("mid equal", 3, d, "d", "", d, 0, 0, 0);
    bad |= Run("mid between d and f", 3, d, "e", "insert;", nw, f, f, 0);   // d has a right child
    bad |= Run("mid before b", 3, d, "a", "unhinted;", un, 0, 0, 0);
    bad |= Run("mid after f", 3, d, "g", "unhinted;", un, 0, 0, 0);
    // Hint f (last): before = d, after = end().
    bad |= Run("last after it", 3, f, "g", "insert;", nw, 0, f, f);
    bad |= Run("last between d and f", 3, f, "e", "insert;", nw, f, f, 0);   // d has a right child
    if (bad) return 1;
    puts("FABLETLC_STLMAP_CCHARSTRING_LONG_INSERT_UNIQUE_HINT_00516AF0_PASS");
    return 0;
}