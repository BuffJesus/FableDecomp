// Behaviour of _Rb_global<bool>::_M_decrement 0x004294E2: header -> rightmost;
// a node with a left subtree -> that subtree's maximum; otherwise the first
// ancestor it is right of.
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

int main()
{
    // Header 0 over the root 4: 2 (children 1, 3) and 6 (children 5, 7).
    B* h = &g_n[0];
    Link(h, &g_n[4], &g_n[1], &g_n[7], _STL::_S_rb_tree_red);
    Link(&g_n[4], h, &g_n[2], &g_n[6], _STL::_S_rb_tree_black);
    Link(&g_n[2], &g_n[4], &g_n[1], &g_n[3], _STL::_S_rb_tree_black);
    Link(&g_n[6], &g_n[4], &g_n[5], &g_n[7], _STL::_S_rb_tree_black);
    Link(&g_n[1], &g_n[2], 0, 0, _STL::_S_rb_tree_red);
    Link(&g_n[3], &g_n[2], 0, 0, _STL::_S_rb_tree_red);
    Link(&g_n[5], &g_n[6], 0, 0, _STL::_S_rb_tree_red);
    Link(&g_n[7], &g_n[6], 0, 0, _STL::_S_rb_tree_red);
    B* n = h;
    for (int want = 7; want >= 1; --want)
    {
        n = G::_M_decrement(n);
        if (n != &g_n[want]) { printf("step to %d got %d\n", want, (int)(n - g_n)); return 1; }
    }
    // A left subtree whose maximum is two right links down.
    Link(&g_n[8], 0, &g_n[9], 0, _STL::_S_rb_tree_black);
    Link(&g_n[9], &g_n[8], 0, &g_n[10], _STL::_S_rb_tree_black);
    Link(&g_n[10], &g_n[9], 0, &g_n[11], _STL::_S_rb_tree_black);
    Link(&g_n[11], &g_n[10], 0, 0, _STL::_S_rb_tree_red);
    if (G::_M_decrement(&g_n[8]) != &g_n[11]) { puts("subtree max"); return 1; }
    // A black node whose grandparent is itself is not the header.
    Link(&g_n[12], &g_n[13], &g_n[14], &g_n[15], _STL::_S_rb_tree_black);
    Link(&g_n[13], &g_n[12], &g_n[12], &g_n[12], _STL::_S_rb_tree_red);
    Link(&g_n[14], &g_n[12], 0, 0, _STL::_S_rb_tree_red);
    if (G::_M_decrement(&g_n[12]) != &g_n[14]) { puts("black root"); return 1; }
    // The same node red: the header rule (its right link).
    g_n[12]._M_color = _STL::_S_rb_tree_red;
    if (G::_M_decrement(&g_n[12]) != &g_n[15]) { puts("red header"); return 1; }
    puts("FABLETLC_RB_GLOBAL_DECREMENT_004294E2_PASS");
    return 0;
}