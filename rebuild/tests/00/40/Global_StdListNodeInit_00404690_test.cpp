#include <stdio.h>
#include <stdlib.h>
void** __fastcall std_list_node_Init(void**,void*,void*);
int main()
{
    void* value=reinterpret_cast<void*>(1); if(std_list_node_Init(&value,0,0)!=&value)return 1;
    if(!value)return 2; void** links=static_cast<void**>(value);
    if(links[0]!=value || links[1]!=value)return 3; free(value);
    printf("STD_LIST_NODE_INIT_TEST PASS\n"); return 0;
}
