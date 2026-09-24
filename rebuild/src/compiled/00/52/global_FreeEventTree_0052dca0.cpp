#include "fable_ui_observer_events.h"
void __fastcall FableUiFreeEventTree(FableUiEventSet* set,void*,FableUiEventTreeNode* node)
{
    while(node)
    {
        FableUiFreeEventTree(set,0,node->Right);
        FableUiEventTreeNode* left=node->Left;
        FableUiFreeEventNode(node);
        node=left;
    }
}
