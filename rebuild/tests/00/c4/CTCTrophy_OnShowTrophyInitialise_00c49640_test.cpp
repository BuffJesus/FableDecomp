#include <cstdio>
struct Sub;
static char* g_p=0;
struct S { char pad[0x70]; int cnt; void M(); };
void __fastcall sub(Sub* p){ g_p=(char*)p; }
void S::M(){ sub((Sub*)((char*)this+0x1c)); this->cnt++; }
int main(){ static S o; o.cnt=5; o.M();
 if(o.cnt!=6||g_p!=(char*)&o+0x1c){ std::printf("00c49640_TEST FAIL\n"); return 1;}
 std::printf("00c49640_TEST PASS\n"); return 0;}