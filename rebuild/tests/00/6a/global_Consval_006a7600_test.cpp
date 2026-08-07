#include <cstdio>
struct Pair; struct Alloc;
static void* g_dst=0; static Pair* g_val=0;
static void cons_construct(void* dst, Pair* val){ g_dst=dst; g_val=val; }
static void Cons_val(Alloc* self, Pair* dst, Pair* val){ (void)dst; cons_construct(*reinterpret_cast<void**>(self), val); }
int main(){ void* inner=(void*)0x55; void* selfblk=&inner; Pair* v=(Pair*)0x66;
 Cons_val((Alloc*)selfblk,(Pair*)0x77,v);
 if(g_dst!=inner||g_val!=v){ std::printf("006a7600_TEST FAIL\n"); return 1;}
 std::printf("006a7600_TEST PASS\n"); return 0;}