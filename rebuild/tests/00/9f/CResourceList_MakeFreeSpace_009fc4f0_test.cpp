#include "fable_resource_list.h"
#include <stdio.h>
#include <string.h>
static unsigned long amount;static long remaining;
void CResourceList::ShrinkResources(unsigned long n){amount=n;AllocatedMemory=remaining;}
int main(){double raw[8];CResourceList* p=reinterpret_cast<CResourceList*>(raw);memset(raw,0,sizeof(raw));p->MaximumMemory=100;
 remaining=75;if(!p->MakeFreeSpace(25) || amount!=25)return 1;
 remaining=76;if(p->MakeFreeSpace(25))return 2;
 p->MaximumMemory=-1;remaining=10;if(!p->MakeFreeSpace(100))return 3;
 puts("FREE_SPACE_PASS");return 0;}