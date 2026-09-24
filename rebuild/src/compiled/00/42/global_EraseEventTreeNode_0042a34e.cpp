#include "fable_ui_observer_events.h"

static bool Black(FableUiEventTreeNode* node) { return !node || node->Colour==1; }
FableUiEventTreeNode* __cdecl FableUiEraseEventTreeNode(FableUiEventTreeNode* removed,
    FableUiEventTreeNode** root,FableUiEventTreeNode** leftmost,FableUiEventTreeNode** rightmost)
{
    FableUiEventTreeNode* replacement=removed;
    FableUiEventTreeNode* child;
    FableUiEventTreeNode* parent;
    if(!replacement->Left) child=replacement->Right;
    else if(!replacement->Right) child=replacement->Left;
    else
    {
        replacement=replacement->Right;
        while(replacement->Left) replacement=replacement->Left;
        child=replacement->Right;
    }
    if(replacement!=removed)
    {
        removed->Left->Parent=replacement;
        replacement->Left=removed->Left;
        if(replacement!=removed->Right)
        {
            parent=replacement->Parent;
            if(child) child->Parent=parent;
            parent->Left=child;
            replacement->Right=removed->Right;
            removed->Right->Parent=replacement;
        }
        else parent=replacement;
        if(*root==removed) *root=replacement;
        else if(removed->Parent->Left==removed) removed->Parent->Left=replacement;
        else removed->Parent->Right=replacement;
        replacement->Parent=removed->Parent;
        unsigned char colour=replacement->Colour;
        replacement->Colour=removed->Colour;
        removed->Colour=colour;
        replacement=removed;
    }
    else
    {
        parent=replacement->Parent;
        if(child) child->Parent=parent;
        if(*root==removed) *root=child;
        else if(removed->Parent->Left==removed) removed->Parent->Left=child;
        else removed->Parent->Right=child;
        if(*leftmost==removed)
        {
            if(!removed->Right) *leftmost=removed->Parent;
            else { *leftmost=child; while((*leftmost)->Left) *leftmost=(*leftmost)->Left; }
        }
        if(*rightmost==removed)
        {
            if(!removed->Left) *rightmost=removed->Parent;
            else { *rightmost=child; while((*rightmost)->Right) *rightmost=(*rightmost)->Right; }
        }
    }
    if(replacement->Colour!=0)
    {
        while(child!=*root && Black(child))
        {
            if(child==parent->Left)
            {
                FableUiEventTreeNode* sibling=parent->Right;
                if(sibling->Colour==0)
                {
                    sibling->Colour=1; parent->Colour=0;
                    FableUiRotateEventTreeLeft(parent,root); sibling=parent->Right;
                }
                if(Black(sibling->Left) && Black(sibling->Right))
                { sibling->Colour=0; child=parent; parent=parent->Parent; }
                else
                {
                    if(Black(sibling->Right))
                    {
                        if(sibling->Left) sibling->Left->Colour=1;
                        sibling->Colour=0; FableUiRotateEventTreeRight(sibling,root); sibling=parent->Right;
                    }
                    sibling->Colour=parent->Colour; parent->Colour=1;
                    if(sibling->Right) sibling->Right->Colour=1;
                    FableUiRotateEventTreeLeft(parent,root); break;
                }
            }
            else
            {
                FableUiEventTreeNode* sibling=parent->Left;
                if(sibling->Colour==0)
                {
                    sibling->Colour=1; parent->Colour=0;
                    FableUiRotateEventTreeRight(parent,root); sibling=parent->Left;
                }
                if(Black(sibling->Right) && Black(sibling->Left))
                { sibling->Colour=0; child=parent; parent=parent->Parent; }
                else
                {
                    if(Black(sibling->Left))
                    {
                        if(sibling->Right) sibling->Right->Colour=1;
                        sibling->Colour=0; FableUiRotateEventTreeLeft(sibling,root); sibling=parent->Left;
                    }
                    sibling->Colour=parent->Colour; parent->Colour=1;
                    if(sibling->Left) sibling->Left->Colour=1;
                    FableUiRotateEventTreeRight(parent,root); break;
                }
            }
        }
        if(child) child->Colour=1;
    }
    return replacement;
}
