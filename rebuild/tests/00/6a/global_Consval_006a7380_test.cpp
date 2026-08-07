#include <cstdio>
static char* g_v=0;
static void __fastcall cons_helper(void* self, void* dst, char* v){ (void)self; (void)dst; g_v=v; }
static void Cons_val(void* self, void* dst, char* val){ cons_helper(self, dst, val + 0xc); }
int main(){ char blk[32]; Cons_val((void*)1,(void*)2,blk); if(g_v!=blk+0xc){ std::printf("006a7380_TEST FAIL\n"); return 1;}
 std::printf("006a7380_TEST PASS\n"); return 0;}