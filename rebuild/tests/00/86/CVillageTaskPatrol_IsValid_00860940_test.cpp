#include "engine/CVillageTaskPatrol.h"
#include <cstdio>
static int probe(void* p){ (void)p; return 5; }
static bool f(CVillageTaskPatrol* self){ return probe(reinterpret_cast<char*>(self)+8) != 0; }
int main(){ CVillageTaskPatrol o; if(!f(&o)){ std::printf("00860940_TEST FAIL\n"); return 1;}
 std::printf("00860940_TEST PASS\n"); return 0;}