// Behaviour of _Rb_global<bool>::_Rebalance 0x0042971D: inserting keys (plain
// BST link, then rebalance) in ascending, descending, zig-zag and mixed orders
// keeps a valid red-black tree: black root, no red node with a red child,
// equal black heights, consistent parents, sorted in-order keys.
#include "fable_stlport_tree.h"
#include <stdio.h>
#include <string.h>

typedef _STL::_Rb_tree_node_base B;
typedef _STL::_Rb_global<bool> G;

static B g_n[16];

static void Link(B* n, B* parent, B* left, B* right, bool color)
{
    n->_M_parent = parent; n->_M_left = left; n->_M_right = right; n->_M_color = color;
}

static B* g_root;
static int Key(B* n) { return (int)(n - g_n); }

static void Insert(int k)
{
    B* z = &g_n[k];
    z->_M_left = z->_M_right = 0;
    if (!g_root) { z->_M_parent = 0; g_root = z; }
    else
    {
        B* y = g_root;
        for (;;)
        {
            B** link = k < Key(y) ? &y->_M_left : &y->_M_right;
            if (!*link) { *link = z; z->_M_parent = y; break; }
            y = *link;
        }
    }
    G::_Rebalance(z, g_root);
}

static int g_last;
static int Valid(B* n, B* parent)
{
    if (!n) return 1;
    if (n->_M_parent != parent && n != g_root) return -1000;
    if (n->_M_color == _STL::_S_rb_tree_red &&
        ((n->_M_left && n->_M_left->_M_color == _STL::_S_rb_tree_red) ||
         (n->_M_right && n->_M_right->_M_color == _STL::_S_rb_tree_red))) return -1000;
    int l = Valid(n->_M_left, n);
    if (Key(n) <= g_last) return -1000;
    g_last = Key(n);
    int r = Valid(n->_M_right, n);
    if (l < 0 || r < 0 || l != r) return -1000;
    return l + (n->_M_color == _STL::_S_rb_tree_black ? 1 : 0);
}

static int Run(const int* keys, int count, const char* what)
{
    g_root = 0;
    for (int i = 0; i < count; ++i) Insert(keys[i]);
    g_last = -1;
    if (g_root->_M_color != _STL::_S_rb_tree_black || Valid(g_root, 0) < 0) { printf("%s\n", what); return 1; }
    return 0;
}

int main()
{
    int up[15], down[15];
    for (int i = 0; i < 15; ++i) { up[i] = i + 1; down[i] = 15 - i; }
    const int zig[15] = { 8, 1, 15, 2, 14, 3, 13, 4, 12, 5, 11, 6, 10, 7, 9 };
    const int mixed[15] = { 5, 3, 4, 10, 12, 11, 1, 2, 15, 13, 14, 7, 6, 9, 8 };
    if (Run(up, 15, "ascending") || Run(down, 15, "descending") || Run(zig, 15, "zig-zag") || Run(mixed, 15, "mixed"))
        return 1;
    // Two keys: no rotation, the child stays red under the black root.
    const int two[2] = { 3, 1 };
    if (Run(two, 2, "two") || g_root != &g_n[3] || g_n[1]._M_color != _STL::_S_rb_tree_red) { puts("two shape"); return 1; }
    // Three ascending keys rotate once: the middle key becomes the root.
    const int three[3] = { 1, 2, 3 };
    if (Run(three, 3, "three") || g_root != &g_n[2]) { puts("three shape"); return 1; }
    // Zig-zag three (3, 1, 2): a double rotation, 2 at the root.
    const int zz[3] = { 3, 1, 2 };
    if (Run(zz, 3, "zz") || g_root != &g_n[2]) { puts("zz shape"); return 1; }
    puts("FABLETLC_RB_GLOBAL_REBALANCE_0042971D_PASS");
    return 0;
}