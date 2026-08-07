#include <cstdio>
struct C { char pad[0x40]; int f40; int DoSizeof(){ return this->f40 + 0x78; } };
int main(){ C o; o.f40=0x100; if(o.DoSizeof()!=0x178){ std::printf("0084d080_TEST FAIL\n"); return 1;} std::printf("0084d080_TEST PASS\n"); return 0;}