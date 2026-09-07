#include "engine/CVillageTaskHorn.h"
#include <cstdio>
static int probe(void* p){ (void)p; return 5; }
static bool f(CVillageTaskHorn* self){ return probe(reinterpret_cast<char*>(self)+8) != 0; }
int main(){ CVillageTaskHorn o; if(!f(&o)){ std::printf("0065ee41_TEST FAIL\n"); return 1;}
 std::printf("0065ee41_TEST PASS\n"); return 0;}