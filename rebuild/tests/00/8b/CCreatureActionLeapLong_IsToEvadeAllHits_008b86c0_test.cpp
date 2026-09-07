#include "engine/CCreatureAction_LeapLong.h"
#include <cstdio>
static bool f(CCreatureAction_LeapLong* self){ return self->b114==0; }
int main(){ CCreatureAction_LeapLong o; o.b114=0; if(!f(&o)){ std::printf("008b86c0_TEST FAIL\n"); return 1;}
 o.b114=5; if(f(&o)){ std::printf("008b86c0_TEST FAIL\n"); return 1;}
 std::printf("008b86c0_TEST PASS\n"); return 0;}