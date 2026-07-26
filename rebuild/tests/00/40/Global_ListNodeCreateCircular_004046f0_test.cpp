#include <stdio.h>
#include <stdlib.h>
void** __fastcall ListNode_Create_Circular(void**,void*,void*);
int main()
{
    void* value=reinterpret_cast<void*>(1); if(ListNode_Create_Circular(&value,0,0)!=&value)return 1;
    if(!value)return 2; void** links=static_cast<void**>(value);
    if(links[0]!=value || links[1]!=value)return 3; free(value);
    printf("LIST_NODE_CREATE_CIRCULAR_TEST PASS\n"); return 0;
}
