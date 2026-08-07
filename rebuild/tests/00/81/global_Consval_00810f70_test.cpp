#include <cstdio>
struct Pair; struct Alloc;
static void* g_v=0;
static void cons_helper(void* p){ g_v=p; }
static void Cons_val(Alloc* self, Pair* dst, char* val){ (void)self; (void)dst; cons_helper(*reinterpret_cast<void**>(val + 8)); }
int main(){ char blk[16]; void* inner=(void*)0x77; *reinterpret_cast<void**>(blk+8)=inner;
 Cons_val((Alloc*)1,(Pair*)2,blk);
 if(g_v!=inner){ std::printf("00810f70_TEST FAIL\n"); return 1;}
 std::printf("00810f70_TEST PASS\n"); return 0;}