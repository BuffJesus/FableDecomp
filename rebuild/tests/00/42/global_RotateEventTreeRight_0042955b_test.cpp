#include "fable_ui_observer_events.h"
#include <stdio.h>
int main()
{
    FableUiEventTreeNode head={},a={},b={},c={};
    FableUiEventTreeNode* root=&a; a.Parent=&head; a.Left=&b; b.Parent=&a; b.Right=&c; c.Parent=&b;
    FableUiRotateEventTreeRight(&a,&root);
    if(root!=&b || b.Parent!=&head || b.Right!=&a || a.Parent!=&b || a.Left!=&c || c.Parent!=&a) return 1;
    puts("PASS_0042955b"); return 0;
}
