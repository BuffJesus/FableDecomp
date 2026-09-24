#include "fable_ui_observer_lifetime.h"
#include <stdio.h>
static FableUiManagerObserverView object;
static unsigned stage;
void __fastcall FableUiDestroyObserverList(FableUiObserverListNode** list,void*)
{
    if(!stage && list==&object.ConcurrentExclusiveObservers) stage=1;
    else if(stage==1 && list==&object.Observers) stage=2;
    else stage=99;
}
int main()
{
    FableUiDestroyObservable(&object,0); if(stage!=2) return 1;
    puts("PASS_0042bec0"); return 0;
}
