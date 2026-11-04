#include "fable_resource_list.h"
#include <stdio.h>
#include <string.h>
static int calls,fail,bad;static long decrement;
bool CResourceList::UnloadNextResource(unsigned long size){++calls;if(size)bad=1;if(fail)return false;AllocatedMemory-=decrement;return true;}
int main(){double raw[8];CResourceList* p=reinterpret_cast<CResourceList*>(raw);memset(raw,0,sizeof(raw));p->MaximumMemory=100;p->AllocatedMemory=90;p->ResourceCount=4;
 p->ShrinkResources(10);if(calls)return 1;
 decrement=10;p->ShrinkResources(25);if(calls!=2 || p->AllocatedMemory!=70)return 2;
 calls=0;decrement=0;p->AllocatedMemory=200;p->ResourceCount=3;p->ShrinkResources(0);if(calls!=3)return 3;
 calls=0;fail=1;p->ShrinkResources(0);if(calls!=1)return 4;
 calls=0;p->ResourceCount=0;p->ShrinkResources(0);if(calls || bad)return 5;
 puts("SHRINK_PASS");return 0;}