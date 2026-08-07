#include "rebuild_abi.h"
#include <cstdio>
struct CEngineInternalPrimitive;
struct CListNode { char pad00[0x30]; CEngineInternalPrimitive *owner; CListNode *next; };
struct CEngineInternalPrimitive { char pad00[8]; CListNode *head; void Update(CListNode *node); };
void CEngineInternalPrimitive::Update(CListNode *node){ node->owner=this; node->next=head; head=node; }
int main(){
    CEngineInternalPrimitive p; CListNode a,b; p.head=&a; a.next=0;
    p.Update(&b);
    if(p.head!=&b||b.next!=&a||b.owner!=&p){std::printf("BAD\n");return 1;}
    std::printf("UPDATE_OK\n"); return 0;
}