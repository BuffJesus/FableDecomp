#include <cstdio>
struct Pair; struct Alloc;
static void* g_v=0;
static void cons_helper(Alloc* self, Pair* dst, void* v){ (void)self; (void)dst; g_v=v; }
static void Cons_val(Alloc* self, Pair* dst, void** val){ cons_helper(self, dst, *val); }
int main(){ void* inner=(void*)0x55; void* pv=&inner; Cons_val((Alloc*)1,(Pair*)2,(void**)pv);
 if(g_v!=inner){ std::printf("00664670_TEST FAIL\n"); return 1;}
 std::printf("00664670_TEST PASS\n"); return 0;}