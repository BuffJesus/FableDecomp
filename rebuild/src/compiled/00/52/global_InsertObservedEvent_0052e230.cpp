#include "fable_ui_observer_events.h"
FableUiEventInsertResult* __fastcall FableUiInsertObservedEvent(FableUiEventSet* set,void*,FableUiEventInsertResult* result,const int* event)
{
    FableUiEventTreeNode* parent=set->Head;
    FableUiEventTreeNode* node=set->Head->Parent;
    while(node)
    {
        parent=node;
        if(*event==node->Event) { result->Node=node; result->Inserted=false; return result; }
        node=*event<node->Event ? node->Left : node->Right;
    }
    FableUiLinkEventNode(set,0,&result->Node,0,parent,event,false);
    result->Inserted=true; return result;
}
