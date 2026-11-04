// Behaviour of _Rb_global<bool>::_Rotate_right 0x0042955B: x's left child y
// takes x's place (root, or x's parent's right or left link), y's right
// subtree becomes x's left, x becomes y's right.
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

static int Check(int x, int parent, int left, int right, const char* what)
{
    B* n = &g_n[x];
    if (n->_M_parent != (parent < 0 ? 0 : &g_n[parent]) || n->_M_left != (left < 0 ? 0 : &g_n[left]) ||
        n->_M_right != (right < 0 ? 0 : &g_n[right])) { printf("%s: node %d\n", what, x); return 1; }
    return 0;
}

int main()
{
    // x=1 is the root: left child 2 (children 3, 4), right child 5.
    B* root = &g_n[1];
    Link(&g_n[1], &g_n[0], &g_n[2], &g_n[5], 0);
    Link(&g_n[2], &g_n[1], &g_n[3], &g_n[4], 0);
    Link(&g_n[3], &g_n[2], 0, 0, 0); Link(&g_n[4], &g_n[2], 0, 0, 0); Link(&g_n[5], &g_n[1], 0, 0, 0);
    G::_Rotate_right(&g_n[1], root);
    if (root != &g_n[2] || Check(2, 0, 3, 1, "root") || Check(1, 2, 4, 5, "root") || Check(4, 1, -1, -1, "root")) return 1;
    // x=6 is its parent 8's right child; y=7 has no right child.
    root = &g_n[8];
    Link(&g_n[8], &g_n[0], 0, &g_n[6], 0);
    Link(&g_n[6], &g_n[8], &g_n[7], 0, 0);
    Link(&g_n[7], &g_n[6], 0, 0, 0);
    G::_Rotate_right(&g_n[6], root);
    if (root != &g_n[8] || g_n[8]._M_right != &g_n[7] || g_n[8]._M_left != 0 || Check(7, 8, -1, 6, "right") ||
        Check(6, 7, -1, -1, "right")) return 1;
    // x=9 is its parent 10's left child.
    Link(&g_n[10], &g_n[0], &g_n[9], 0, 0);
    Link(&g_n[9], &g_n[10], &g_n[11], 0, 0);
    Link(&g_n[11], &g_n[9], 0, &g_n[12], 0);
    Link(&g_n[12], &g_n[11], 0, 0, 0);
    G::_Rotate_right(&g_n[9], root);
    if (root != &g_n[8] || g_n[10]._M_left != &g_n[11] || g_n[10]._M_right != 0 || Check(11, 10, -1, 9, "left") ||
        Check(9, 11, 12, -1, "left") || Check(12, 9, -1, -1, "left")) return 1;
    puts("FABLETLC_RB_GLOBAL_ROTATE_RIGHT_0042955B_PASS");
    return 0;
}
