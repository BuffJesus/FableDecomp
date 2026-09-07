#include "engine/CTCHeroRangedAttacks.h"
#include <cstdio>
struct CDefPointeeBase { char pad[0x136]; unsigned char b136; };
static bool f(CTCHeroRangedAttacks* self){ return self->PHeroCombatDef_Object->b136 ? true : false; }
int main(){ CDefPointeeBase inr; inr.b136=1; CTCHeroRangedAttacks o; o.PHeroCombatDef_Object=&inr; if(!f(&o)){ std::printf("00789ba0_TEST FAIL\n"); return 1;}
 inr.b136=0; if(f(&o)){ std::printf("00789ba0_TEST FAIL\n"); return 1;}
 std::printf("00789ba0_TEST PASS\n"); return 0;}