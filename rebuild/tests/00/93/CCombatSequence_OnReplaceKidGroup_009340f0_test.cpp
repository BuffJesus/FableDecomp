#include <cstdio>
struct S { char pad[0x2C]; int result; };
static int re_call(S* self){ (void)self; return 0x1234; }
static void OnReplaceKidGroup(S* self){ self->result = re_call(self); }
int main(){ S obj; obj.result=0; OnReplaceKidGroup(&obj);
 if(obj.result!=0x1234){ std::printf("009340f0_TEST FAIL\n"); return 1;}
 std::printf("009340f0_TEST PASS\n"); return 0;}