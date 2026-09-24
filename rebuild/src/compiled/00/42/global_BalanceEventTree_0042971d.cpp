#include "fable_ui_observer_events.h"
void __cdecl FableUiBalanceEventTree(FableUiEventTreeNode* node,FableUiEventTreeNode** root)
{
    node->Colour=0;
    while(node!=*root && node->Parent->Colour==0)
    {
        FableUiEventTreeNode* parent=node->Parent;
        FableUiEventTreeNode* grandparent=parent->Parent;
        bool parentOnLeft=parent==grandparent->Left;
        FableUiEventTreeNode* uncle=parentOnLeft ? grandparent->Right : grandparent->Left;
        if(uncle && uncle->Colour==0)
        {
            parent->Colour=1; uncle->Colour=1; grandparent->Colour=0; node=grandparent;
        }
        else if(parentOnLeft)
        {
            if(node==parent->Right) { node=parent; FableUiRotateEventTreeLeft(node,root); }
            node->Parent->Colour=1; node->Parent->Parent->Colour=0;
            FableUiRotateEventTreeRight(node->Parent->Parent,root);
        }
        else
        {
            if(node==parent->Left) { node=parent; FableUiRotateEventTreeRight(node,root); }
            node->Parent->Colour=1; node->Parent->Parent->Colour=0;
            FableUiRotateEventTreeLeft(node->Parent->Parent,root);
        }
    }
    (*root)->Colour=1;
}
