#include "fable_ui_observer.h"
#include <stdio.h>
static unsigned step;
static FableUiComponentDrawView* expected;
static void __fastcall Request(FableUiComponentDrawView* component,void*,unsigned state)
{ if(component==expected && state==2 && step==0) step=1; else step=99; }
void __fastcall FableUiRemoveObserverRecursive(FableUiComponentDrawView* component,void*)
{ if(component==expected && step==1) step=2; else step=99; }
int main()
{
    FableUiComponentDrawView component={}; FableUiComponentDrawVtable table={};
    component.Vtable=&table; table.RequestState=Request; expected=&component;
    FableUiDie(&component,0);
    if(step!=2) return 1;
    puts("OK_0x00530720"); return 0;
}
