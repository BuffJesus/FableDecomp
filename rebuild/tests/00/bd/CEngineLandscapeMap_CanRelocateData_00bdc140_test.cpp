#include <cstdio>
struct S { char pad[0x5C]; unsigned char f; };
static bool CanRelocateData(S* self){ return self->f == 0; }
int main(){ S obj; obj.f=0; if(!CanRelocateData(&obj)){ std::printf("00bdc140_TEST FAIL\n"); return 1;}
 obj.f=7; if(CanRelocateData(&obj)){ std::printf("00bdc140_TEST FAIL\n"); return 1;}
 std::printf("00bdc140_TEST PASS\n"); return 0;}