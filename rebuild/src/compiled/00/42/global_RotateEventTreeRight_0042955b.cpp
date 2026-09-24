#include "fable_ui_observer_events.h"
void __cdecl FableUiRotateEventTreeRight(FableUiEventTreeNode* node,FableUiEventTreeNode** root)
{
    FableUiEventTreeNode* left=node->Left;
    node->Left=left->Right;
    if(left->Right) left->Right->Parent=node;
    left->Parent=node->Parent;
    if(node==*root) *root=left;
    else if(node==node->Parent->Right) node->Parent->Right=left;
    else node->Parent->Left=left;
    left->Right=node; node->Parent=left;
}
