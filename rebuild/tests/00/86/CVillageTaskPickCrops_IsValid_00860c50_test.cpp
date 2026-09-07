#include "engine/CVillageTaskPickCrops.h"
#include <cstdio>
static int probe(void* p){ (void)p; return 5; }
static bool f(CVillageTaskPickCrops* self){ return probe(reinterpret_cast<char*>(self)+8) != 0; }
int main(){ CVillageTaskPickCrops o; if(!f(&o)){ std::printf("00860c50_TEST FAIL\n"); return 1;}
 std::printf("00860c50_TEST PASS\n"); return 0;}