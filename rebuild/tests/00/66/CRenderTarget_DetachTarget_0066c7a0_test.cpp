#include <cstdio>
static void* g=0;
struct S { char pad[0x24]; void* f24; void* f28; };
static void __fastcall dt_helper(void* p){ g=p; }
static void DetachTarget(S* self){ if(self->f28) dt_helper(reinterpret_cast<char*>(self)+0x24); }
int main(){ S o; o.f28=0; DetachTarget(&o); if(g){ std::printf("0066c7a0_TEST FAIL\n"); return 1;}
 int m; o.f28=&m; DetachTarget(&o); if(g!=(char*)&o+0x24){ std::printf("0066c7a0_TEST FAIL\n"); return 1;}
 std::printf("0066c7a0_TEST PASS\n"); return 0;}