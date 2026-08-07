#include <cstdio>
struct S { char pad[0x28]; int result; };
static int re_call(S* self){ (void)self; return 0x1234; }
static void OnStart(S* self){ self->result = re_call(self); }
int main(){ S obj; obj.result=0; OnStart(&obj);
 if(obj.result!=0x1234){ std::printf("00906fc0_TEST FAIL\n"); return 1;}
 std::printf("00906fc0_TEST PASS\n"); return 0;}