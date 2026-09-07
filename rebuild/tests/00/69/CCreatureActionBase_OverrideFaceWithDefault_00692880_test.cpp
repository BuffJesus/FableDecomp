#include "engine/CCreatureActionBase.h"
#include <cstdio>
struct CAnimationEntry; static unsigned int probe(CAnimationEntry* p){ (void)p; return 0x10; }
static bool f(CCreatureActionBase* self){ return (probe(self->PAnimEntry_Data) >> 4) & 1; }
int main(){ CCreatureActionBase o; o.PAnimEntry_Data=0; if(!f(&o)){ std::printf("00692880_TEST FAIL\n"); return 1;}
 std::printf("00692880_TEST PASS\n"); return 0;}