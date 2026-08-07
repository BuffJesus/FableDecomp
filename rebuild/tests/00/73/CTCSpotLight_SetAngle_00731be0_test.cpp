#include <cstdio>
struct S { char pad[0x10]; float* f10; char pad2[8]; unsigned char b1c; };
static void f(S* self, float a){ float* p=self->f10; self->b1c=1; *p=a; }
int main(){ float v=0; S o; o.f10=&v; o.b1c=0; f(&o, 2.5f);
 if(v!=2.5f||o.b1c!=1){ std::printf("00731be0_TEST FAIL\n"); return 1;}
 std::printf("00731be0_TEST PASS\n"); return 0;}