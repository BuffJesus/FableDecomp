#include <cstdio>
static int g=0; static char g_vtbl[8]; static void __fastcall vb_helper(void* p){ (void)p; g=1; }
static void Dtor(void* self){ *reinterpret_cast<void**>(reinterpret_cast<char*>(self)+0x1f0) = (void*)g_vtbl; vb_helper(self); }
int main(){ char blk[0x200]; Dtor(blk); if(*reinterpret_cast<void**>(blk+0x1f0)!=(void*)g_vtbl||!g){ std::printf("00c3f630_TEST FAIL\n"); return 1;}
 std::printf("00c3f630_TEST PASS\n"); return 0;}