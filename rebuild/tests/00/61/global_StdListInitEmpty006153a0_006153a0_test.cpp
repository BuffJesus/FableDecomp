#include <stdlib.h>
#include <stdio.h>
struct Node { Node* next; Node* previous; unsigned long reserved; };
static void** Run(void** owner) { *owner=0; Node* node=(Node*)malloc(0x0c); if(!node)return 0; node->next=node; node->previous=node; *owner=node; return owner; }
int main(){void* owner=(void*)1;if(Run(&owner)!=&owner||!owner)return 1;Node* node=(Node*)owner;if(node->next!=node||node->previous!=node)return 2;free(node);puts("LIST_SENTINEL_CTOR_006153a0_PASS");return 0;}