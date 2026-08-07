#include <cstdio>
struct S { char pad[0x38]; char* f38; };
static char* f(S* self){ char* p=self->f38; return p ? p+0xC : 0; }
int main(){ char buf[32]; S o; o.f38=buf; if(f(&o)!=buf+0xC){ std::printf("0088b7d0_TEST FAIL\n"); return 1;}
 o.f38=0; if(f(&o)!=0){ std::printf("0088b7d0_TEST FAIL\n"); return 1;}
 std::printf("0088b7d0_TEST PASS\n"); return 0;}