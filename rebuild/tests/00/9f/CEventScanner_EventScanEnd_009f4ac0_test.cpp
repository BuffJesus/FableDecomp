#include <cstdio>
struct S { char pad[0x20]; int cnt; };
static void EventScanEnd(S* self, unsigned char* p){ *p=0; self->cnt--; }
int main(){ S obj; obj.cnt=5; unsigned char flag=9; EventScanEnd(&obj,&flag);
 if(flag!=0||obj.cnt!=4){ std::printf("009f4ac0_TEST FAIL\n"); return 1;}
 std::printf("009f4ac0_TEST PASS\n"); return 0;}