#include "fable_ui_observer_lifetime.h"
#include <stdio.h>
static FableUiObserverListNode node;
static FableUiObserverListNode* head;
static bool valid;
void* FableUiAllocateObserverNode(unsigned size) { valid=size==12 && head==0; return &node; }
int main()
{
    node.Observer=reinterpret_cast<FableUiObserverInterfaceView*>(0x12345678); head=&node;
    if(FableUiConstructObserverList(&head,0,0)!=&head || !valid || head!=&node || head->Next!=head || head->Previous!=head || node.Observer!=reinterpret_cast<FableUiObserverInterfaceView*>(0x12345678)) return 1;
    puts("DLIST_INITIALIZE_EMPTY_PASS"); return 0;
}
