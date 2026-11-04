#pragma optimize("s", on)
#include "fable_stlport_tree.h"
namespace _STL {
template<class Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<Dummy>::_Rebalance_for_erase(
    _Rb_tree_node_base* z,_Rb_tree_node_base*& root,
    _Rb_tree_node_base*& leftmost,_Rb_tree_node_base*& rightmost) {
    _Rb_tree_node_base* y=z;
    _Rb_tree_node_base* x=0;
    _Rb_tree_node_base* x_parent=0;
    if(y->_M_left==0) x=y->_M_right;
    else if(y->_M_right==0) x=y->_M_left;
    else {
        y=y->_M_right;
        while(y->_M_left) y=y->_M_left;
        x=y->_M_right;
    }
    if(y!=z) {
        z->_M_left->_M_parent=y;
        y->_M_left=z->_M_left;
        if(y!=z->_M_right) {
            x_parent=y->_M_parent;
            if(x)x->_M_parent=y->_M_parent;
            y->_M_parent->_M_left=x;
            y->_M_right=z->_M_right;
            z->_M_right->_M_parent=y;
        } else x_parent=y;
        if(root==z)root=y;
        else if(z->_M_parent->_M_left==z)z->_M_parent->_M_left=y;
        else z->_M_parent->_M_right=y;
        y->_M_parent=z->_M_parent;
        _Rb_tree_Color_type color=y->_M_color;y->_M_color=z->_M_color;z->_M_color=color;
        y=z;
    } else {
        x_parent=y->_M_parent;
        if(x)x->_M_parent=y->_M_parent;
        if(root==z)root=x;
        else if(z->_M_parent->_M_left==z)z->_M_parent->_M_left=x;
        else z->_M_parent->_M_right=x;
        if(leftmost==z) {
            if(z->_M_right==0)leftmost=z->_M_parent;
            else leftmost=_Rb_tree_node_base::_S_minimum(x);
        }
        if(rightmost==z) {
            if(z->_M_left==0)rightmost=z->_M_parent;
            else rightmost=_Rb_tree_node_base::_S_maximum(x);
        }
    }
    if(y->_M_color!=_S_rb_tree_red) {
        while(x!=root&&(x==0||x->_M_color==_S_rb_tree_black)) {
            if(x==x_parent->_M_left) {
                _Rb_tree_node_base* w=x_parent->_M_right;
                if(w->_M_color==_S_rb_tree_red) {
                    w->_M_color=_S_rb_tree_black;x_parent->_M_color=_S_rb_tree_red;
                    _Rotate_left(x_parent,root);w=x_parent->_M_right;
                }
                if((w->_M_left==0||w->_M_left->_M_color==_S_rb_tree_black)&&
                   (w->_M_right==0||w->_M_right->_M_color==_S_rb_tree_black)) {
                    w->_M_color=_S_rb_tree_red;x=x_parent;x_parent=x_parent->_M_parent;
                } else {
                    if(w->_M_right==0||w->_M_right->_M_color==_S_rb_tree_black) {
                        if(w->_M_left)w->_M_left->_M_color=_S_rb_tree_black;
                        w->_M_color=_S_rb_tree_red;_Rotate_right(w,root);w=x_parent->_M_right;
                    }
                    w->_M_color=x_parent->_M_color;x_parent->_M_color=_S_rb_tree_black;
                    if(w->_M_right)w->_M_right->_M_color=_S_rb_tree_black;
                    _Rotate_left(x_parent,root);break;
                }
            } else {
                _Rb_tree_node_base* w=x_parent->_M_left;
                if(w->_M_color==_S_rb_tree_red) {
                    w->_M_color=_S_rb_tree_black;x_parent->_M_color=_S_rb_tree_red;
                    _Rotate_right(x_parent,root);w=x_parent->_M_left;
                }
                if((w->_M_right==0||w->_M_right->_M_color==_S_rb_tree_black)&&
                   (w->_M_left==0||w->_M_left->_M_color==_S_rb_tree_black)) {
                    w->_M_color=_S_rb_tree_red;x=x_parent;x_parent=x_parent->_M_parent;
                } else {
                    if(w->_M_left==0||w->_M_left->_M_color==_S_rb_tree_black) {
                        if(w->_M_right)w->_M_right->_M_color=_S_rb_tree_black;
                        w->_M_color=_S_rb_tree_red;_Rotate_left(w,root);w=x_parent->_M_left;
                    }
                    w->_M_color=x_parent->_M_color;x_parent->_M_color=_S_rb_tree_black;
                    if(w->_M_left)w->_M_left->_M_color=_S_rb_tree_black;
                    _Rotate_right(x_parent,root);break;
                }
            }
        }
        if(x)x->_M_color=_S_rb_tree_black;
    }
    return y;
}
template _Rb_tree_node_base* __cdecl _Rb_global<bool>::_Rebalance_for_erase(_Rb_tree_node_base*,_Rb_tree_node_base*&,_Rb_tree_node_base*&,_Rb_tree_node_base*&);
}
