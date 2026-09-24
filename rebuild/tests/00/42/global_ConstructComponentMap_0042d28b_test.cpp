#include "fable_ui_manager_construction.h"
#include <stdio.h>
#include <string.h>
static unsigned char node[28];
static unsigned requested;
void* FableUiAllocateManagerStorage(unsigned bytes) { requested=bytes; return node; }
int main()
{
    FableUiComponentMapStorage map; memset(&map,0xA5,sizeof(map)); memset(node,0xCD,sizeof(node));
    if(FableUiConstructComponentMap(&map,0)!=&map || requested!=28 || map.Count || map.Head!=reinterpret_cast<FableUiEventTreeNode*>(node) || map.Head->Colour || map.Head->Parent || map.Head->Left!=map.Head || map.Head->Right!=map.Head || map.Unrecovered08[0]!=0xA5) return 1;
    puts("UI_MANAGER_MAP_CTOR PASS"); return 0;
}
