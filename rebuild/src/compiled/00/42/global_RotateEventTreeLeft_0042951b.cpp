#include "fable_ui_observer_events.h"
void __cdecl FableUiRotateEventTreeLeft(FableUiEventTreeNode* node,FableUiEventTreeNode** root)
{
    FableUiEventTreeNode* right=node->Right;
    node->Right=right->Left;
    if(right->Left) right->Left->Parent=node;
    right->Parent=node->Parent;
    if(node==*root) *root=right;
    else if(node==node->Parent->Left) node->Parent->Left=right;
    else node->Parent->Right=right;
    right->Left=node; node->Parent=right;
}
