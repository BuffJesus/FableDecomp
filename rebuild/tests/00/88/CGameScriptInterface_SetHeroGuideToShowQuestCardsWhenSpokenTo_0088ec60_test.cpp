#include <cstdio>
struct Inner { char pad[0xda]; unsigned char b_da; };
struct S { char pad[4]; Inner* f4; void Set(unsigned char v); };
void S::Set(unsigned char v){ this->f4->b_da = v; }
int main(){ Inner inr; S o; o.f4=&inr; o.Set(1); if(inr.b_da!=1){ std::printf("0088ec60_TEST FAIL\n"); return 1;}
 std::printf("0088ec60_TEST PASS\n"); return 0;}