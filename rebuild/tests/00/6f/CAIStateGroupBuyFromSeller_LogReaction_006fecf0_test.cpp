#include <cstdio>
struct S; static int __fastcall lr_helper(S*);
struct CThingAICreature; struct S { char pad[0xC]; int fC; void LogReaction(const CThingAICreature&, bool); };
static int __fastcall lr_helper(S*){ return 55; }
void S::LogReaction(const CThingAICreature&, bool){ this->fC = lr_helper(this); }
int main(){ S o; o.fC=0; CThingAICreature* c=0; o.LogReaction(*c, true);
 if(o.fC!=55){ std::printf("006fecf0_TEST FAIL\n"); return 1;}
 std::printf("006fecf0_TEST PASS\n"); return 0;}