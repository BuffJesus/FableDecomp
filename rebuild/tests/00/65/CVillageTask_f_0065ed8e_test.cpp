#include "engine/CVillageTaskTorch.h"
#include <cstdio>
static int probe(void* p){ (void)p; return 5; }
static bool f(CVillageTaskTorch* self){ return probe(reinterpret_cast<char*>(self)+0x10) != 0; }
int main(){ CVillageTaskTorch o; if(!f(&o)){ std::printf("0065ed8e_TEST FAIL\n"); return 1;}
 std::printf("0065ed8e_TEST PASS\n"); return 0;}