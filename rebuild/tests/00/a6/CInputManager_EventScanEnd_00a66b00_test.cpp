#include <cstdio>
struct S { char pad[0x3408]; int cnt; void M(char* p); };
void S::M(char* p){ *p=0; this->cnt--; }
int main(){ static S o; o.cnt=5; char c=7; o.M(&c);
 if(c!=0||o.cnt!=4){ std::printf("00a66b00_TEST FAIL\n"); return 1;}
 std::printf("00a66b00_TEST PASS\n"); return 0;}