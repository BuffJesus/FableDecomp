#include "fable_ui_observer_events.h"
FableUiEventTreeNode** __fastcall FableUiLinkEventNode(FableUiEventSet* set,void*,FableUiEventTreeNode** result,
    FableUiEventTreeNode* position,FableUiEventTreeNode* parent,const int* event,bool forceRight)
{
    bool left=parent==set->Head || (!forceRight && (position || *event<parent->Event));
    FableUiEventTreeNode* node=static_cast<FableUiEventTreeNode*>(FableUiAllocateEventNode(20));
    node->Event=*event;
    if(left)
    {
        parent->Left=node;
        if(parent==set->Head) { set->Head->Parent=node; set->Head->Right=node; }
        else if(parent==set->Head->Left) set->Head->Left=node;
    }
    else
    {
        parent->Right=node;
        if(parent==set->Head->Right) set->Head->Right=node;
    }
    node->Parent=parent; node->Left=0; node->Right=0;
    FableUiBalanceEventTree(node,&set->Head->Parent);
    ++set->Count; *result=node; return result;
}
