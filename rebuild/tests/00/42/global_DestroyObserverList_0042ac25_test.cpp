#include "fable_ui_observer_lifetime.h"
#include <stdio.h>
static FableUiObserverListNode node;
static unsigned stage;
void __fastcall FableUiClearObserverList(FableUiObserverListNode** list,void*) { if(*list==&node && !stage) stage=1; }
void FableUiFreeObserverNode(void* p) { if(p==&node && stage==1) stage=2; }
int main()
{
    FableUiObserverListNode* head=&node; FableUiDestroyObserverList(&head,0);
    if(stage!=2 || head!=&node) return 1;
    puts("PASS_0042ac25"); return 0;
}
