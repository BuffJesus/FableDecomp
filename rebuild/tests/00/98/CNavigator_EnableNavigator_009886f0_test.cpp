#include <cstdio>
struct S { char pad[0x393]; unsigned char f; };
static void EnableNavigator(S* self, unsigned char v){ self->f=v; }
int main(){ S obj; obj.f=0; EnableNavigator(&obj,1);
 if(obj.f!=1){ std::printf("009886f0_TEST FAIL\n"); return 1;}
 std::printf("009886f0_TEST PASS\n"); return 0;}