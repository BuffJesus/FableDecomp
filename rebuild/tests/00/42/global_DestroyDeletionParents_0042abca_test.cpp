#include "fable_ui_deletion.h"
#include <stdio.h>
static void* freed[3];
static unsigned count;
void __cdecl FableUiListFree(void* value) { if(count<3) freed[count]=value; ++count; }
int main()
{
    FableUiDeletionParentNode nodes[3];
    for(unsigned i=0;i<3;++i) { nodes[i].Next=&nodes[(i+1)%3]; nodes[i].Previous=&nodes[(i+2)%3]; }
    FableUiDeletionParentNode* head=nodes;
    FableUiDestroyDeletionParents(&head,0);
    if(count!=3 || freed[0]!=nodes+1 || freed[1]!=nodes+2 || freed[2]!=nodes || nodes[0].Next!=nodes || nodes[0].Previous!=nodes) return 1;
    puts("UI_DELETION_DESTROY PASS"); return 0;
}
