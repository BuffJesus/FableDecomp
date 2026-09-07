#include "engine/CSpawnedFuncBase.h"
#include <cstdio>
struct CPredicateBase { virtual bool v0(){ return false; } };
static bool f(CSpawnedFuncBase* self){ CPredicateBase* p=self->PPredicate_Data; if(!p) return true; return p->v0(); }
int main(){ CSpawnedFuncBase o; o.PPredicate_Data=0; if(!f(&o)){ std::printf("00f35a00_TEST FAIL\n"); return 1;}
 CPredicateBase p; o.PPredicate_Data=&p; if(f(&o)){ std::printf("00f35a00_TEST FAIL\n"); return 1;}
 std::printf("00f35a00_TEST PASS\n"); return 0;}