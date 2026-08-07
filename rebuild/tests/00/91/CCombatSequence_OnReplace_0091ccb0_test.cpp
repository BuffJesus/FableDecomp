#include <cstdio>
struct S { char pad[0x24]; int result; };
static int re_call(S* self){ (void)self; return 0x1234; }
static void OnReplace(S* self){ self->result = re_call(self); }
int main(){ S obj; obj.result=0; OnReplace(&obj);
 if(obj.result!=0x1234){ std::printf("0091ccb0_TEST FAIL\n"); return 1;}
 std::printf("0091ccb0_TEST PASS\n"); return 0;}