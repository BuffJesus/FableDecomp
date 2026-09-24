#include "fable_ui_observer_events.h"
FableUiEventTreeNode** __fastcall FableUiFindEventInRange(FableUiEventTreeNode** result,const int* event,
    FableUiEventTreeNode* entry,FableUiEventTreeNode* end,const void*)
{
    while(entry!=end && entry->Event!=*event)
    {
        if(entry->Right)
        {
            entry=entry->Right;
            while(entry->Left) entry=entry->Left;
        }
        else
        {
            FableUiEventTreeNode* parent=entry->Parent;
            while(entry==parent->Right) { entry=parent; parent=parent->Parent; }
            if(entry->Right!=parent) entry=parent;
        }
    }
    *result=entry;
    return result;
}
