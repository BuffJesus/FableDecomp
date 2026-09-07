#include <stdio.h>
struct Mods { int calls; int includePermanent; void Remove(bool value){calls++;includePermanent=value;} };
struct Entry { int key; Mods* mods; };
struct Hero { unsigned flags28; unsigned char flags91; Entry* lower; Entry* end; };
static void Run(Hero* hero){if(!hero||(hero->flags91&1)||!(hero->flags28&0x40000000))return;Entry* e=hero->lower;if(e==hero->end||e->key>0x5e)e=hero->end;e->mods->Remove(false);}
int main(){Mods direct={0,1},sentinel={0,1};Entry entries[2]={{0x5e,&direct},{0x99,&sentinel}};Hero h={0x40000000,0,&entries[0],&entries[1]};Run(&h);if(direct.calls!=1||direct.includePermanent)return 1;h.lower=h.end;Run(&h);if(sentinel.calls!=1||sentinel.includePermanent)return 2;h.lower=&entries[0];entries[0].key=0x5f;Run(&h);if(sentinel.calls!=2)return 3;h.flags91=1;Run(&h);if(sentinel.calls!=2)return 4;h.flags91=0;h.flags28=0;Run(&h);Run(0);if(sentinel.calls!=2)return 5;puts("REMOVE_HERO_HAIRSTYLE_PASS");return 0;}