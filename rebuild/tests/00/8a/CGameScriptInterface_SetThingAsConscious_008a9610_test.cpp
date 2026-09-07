#include <stdio.h>
struct Creature { unsigned flags24; unsigned char flags6c; unsigned char flags91; };
struct Thing { bool valid; Creature* creature; };
static int logv[4],logn;static const void* seenReason;
static void Log(int v){logv[logn++]=v;}
static void Run(const Thing& t,bool conscious,const void* reason){if(!t.valid)return;Creature* c=t.creature;if(!c||(c->flags91&1)||!(c->flags6c&8)||!(c->flags24&0x20000))return;if(conscious)Log(1);else{Log(2);seenReason=reason;}Log(3);Log(4);}
int main(){Creature c={0x20000,8,0};Thing t={true,&c};int reason=9;Run(t,true,&reason);if(logn!=3||logv[0]!=1||logv[1]!=3||logv[2]!=4)return 1;logn=0;seenReason=0;Run(t,false,&reason);if(logn!=3||logv[0]!=2||logv[1]!=3||logv[2]!=4||seenReason!=&reason)return 2;Thing bad=t;bad.valid=false;logn=0;Run(bad,true,&reason);if(logn)return 3;bad=t;bad.creature=0;Run(bad,true,&reason);if(logn)return 4;c.flags91=1;Run(t,true,&reason);if(logn)return 5;c.flags91=0;c.flags6c=0;Run(t,true,&reason);if(logn)return 6;c.flags6c=8;c.flags24=0;Run(t,true,&reason);if(logn)return 7;puts("SET_THING_AS_CONSCIOUS_PASS");return 0;}