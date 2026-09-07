#include <stdio.h>
struct Node { Node* next; };
struct Stats { char pad[0x1d8]; Node* marriages; };
struct Entry { int key; Stats* stats; };
struct Target { unsigned char flags20; unsigned char flags91; Entry* lower; Entry* end; };
static bool Run(Target* target){if(!target)return false;if(target->flags91&1)return false;if(!(target->flags20&0x10))return false;Entry* entry=target->lower;if(entry==target->end||entry->key>4)entry=target->end;Node* head=entry->stats->marriages;return head->next!=head;}
int main(){Node empty,member,full;empty.next=&empty;full.next=&member;member.next=&full;Stats direct,sentinel;direct.marriages=&empty;sentinel.marriages=&full;Entry entries[2]={{4,&direct},{99,&sentinel}};Target t={0x10,0,&entries[0],&entries[1]};if(Run(&t))return 1;direct.marriages=&full;if(!Run(&t))return 2;t.lower=t.end;if(!Run(&t))return 3;t.lower=&entries[0];entries[0].key=5;if(!Run(&t))return 4;t.flags91=1;if(Run(&t))return 5;t.flags91=0;t.flags20=0;if(Run(&t)||Run(0))return 6;puts("GET_HERO_HAS_MARRIED_PASS");return 0;}