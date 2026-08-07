#include <cstdio>
static int g_called=0;
struct Base { void BaseDtor(); };
void Base::BaseDtor(){ g_called=1; }
static char g_vtbl[8];
static void Dtor(void* self){ *(void**)self = (void*)g_vtbl; ((Base*)((char*)self + 4))->BaseDtor(); }
int main(){ char blk[16]; Dtor(blk); if(*(void**)blk!=(void*)g_vtbl||!g_called){ std::printf("00c2a2d0_TEST FAIL\n"); return 1;}
 std::printf("00c2a2d0_TEST PASS\n"); return 0;}