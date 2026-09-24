#include "fable_ui_observer_lifetime.h"
#include <stdio.h>
static FableUiObserverListNode nodes[3];
static unsigned freed;
void FableUiFreeObserverNode(void* p) { if(p==nodes+freed+1) ++freed; }
int main()
{
    FableUiObserverListNode* head=nodes;
    for(unsigned i=0;i<3;++i) { nodes[i].Next=nodes+(i+1)%3; nodes[i].Previous=nodes+(i+2)%3; }
    FableUiClearObserverList(&head,0);
    if(freed!=2 || head!=nodes || head->Next!=head || head->Previous!=head) return 1;
    FableUiClearObserverList(&head,0); if(freed!=2) return 2;
    puts("PASS_0042a1e3"); return 0;
}
