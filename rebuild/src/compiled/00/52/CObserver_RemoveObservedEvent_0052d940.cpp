#include "fable_ui_observer_events.h"
void __fastcall FableUiRemoveObservedEvent(FableUiObserverEventsView* observer,void*,int event)
{
    FableUiEventSet& set=observer->EventsToObserve;
    FableUiEventTreeNode* found;
    FableUiFindEventInRange(&found,&event,set.Head->Left,set.Head,0);
    if(found==set.Head) return;
    FableUiEventTreeNode* removed=FableUiEraseEventTreeNode(found,&set.Head->Parent,&set.Head->Left,&set.Head->Right);
    if(removed) FableUiFreeEventNode(removed);
    --set.Count;
}
