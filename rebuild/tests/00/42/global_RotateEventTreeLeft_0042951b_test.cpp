#include "fable_ui_observer_events.h"
#include <stdio.h>
int main()
{
    FableUiEventTreeNode head={},a={},b={},c={};
    FableUiEventTreeNode* root=&a; a.Parent=&head; a.Right=&b; b.Parent=&a; b.Left=&c; c.Parent=&b;
    FableUiRotateEventTreeLeft(&a,&root);
    if(root!=&b || b.Parent!=&head || b.Left!=&a || a.Parent!=&b || a.Right!=&c || c.Parent!=&a) return 1;
    puts("PASS_0042951b"); return 0;
}
