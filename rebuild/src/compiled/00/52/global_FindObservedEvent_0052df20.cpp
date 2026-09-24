#include "fable_ui_observer_events.h"
FableUiEventTreeNode** __fastcall FableUiFindObservedEvent(FableUiEventSet* set,void*,FableUiEventTreeNode** result,const int* event)
{
    FableUiEventTreeNode* candidate=set->Head;
    FableUiEventTreeNode* node=set->Head->Parent;
    while(node)
    {
        if(node->Event>=*event) { candidate=node; node=node->Left; }
        else node=node->Right;
    }
    *result=candidate==set->Head || *event<candidate->Event ? set->Head : candidate;
    return result;
}
