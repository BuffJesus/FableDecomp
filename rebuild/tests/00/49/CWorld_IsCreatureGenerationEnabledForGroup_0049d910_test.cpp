#include <cstdio>
struct S { char pad[0xd0]; unsigned int fd0; int M(unsigned int a0); };
int S::M(unsigned int a0){ unsigned int v = this->fd0 & a0; return v == 0; }
int main(){ S o; o.fd0=0x4; if(o.M(0x1)!=1){ std::printf("0049d910_TEST FAIL\n"); return 1;}
 if(o.M(0x4)!=0){ std::printf("0049d910_TEST FAIL\n"); return 1;}
 std::printf("0049d910_TEST PASS\n"); return 0;}