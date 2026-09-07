#include <stdio.h>
struct Stats { bool result; int calls; bool HasCurrentMarriage(){calls++;return result;} };
struct Entry { int key; Stats* stats; };
struct Target { unsigned char flags20; unsigned char flags91; Entry* lower; Entry* end; };
static bool Run(Target* target){if(!target)return false;if(target->flags91&1)return false;if(!(target->flags20&0x10))return false;Entry* entry=target->lower;if(entry==target->end||entry->key>4)entry=target->end;return entry->stats->HasCurrentMarriage();}
int main(){Stats direct={true,0},sentinel={false,0};Entry entries[2]={{4,&direct},{99,&sentinel}};Target t={0x10,0,&entries[0],&entries[1]};if(!Run(&t)||direct.calls!=1)return 1;t.lower=t.end;if(Run(&t)||sentinel.calls!=1)return 2;sentinel.result=true;t.lower=&entries[0];entries[0].key=5;if(!Run(&t)||sentinel.calls!=2)return 3;t.flags91=1;if(Run(&t))return 4;t.flags91=0;t.flags20=0;if(Run(&t)||Run(0))return 5;puts("GET_HERO_HAS_CURRENT_MARRIAGE_PASS");return 0;}