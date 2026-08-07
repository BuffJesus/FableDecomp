#include <cstdio>
struct S { char pad[0x14]; int result; };
static int re_call(S* self){ (void)self; return 0x1234; }
static void BoughtSomething(S* self){ self->result = re_call(self); }
int main(){ S obj; obj.result=0; BoughtSomething(&obj);
 if(obj.result!=0x1234){ std::printf("00919a30_TEST FAIL\n"); return 1;}
 std::printf("00919a30_TEST PASS\n"); return 0;}