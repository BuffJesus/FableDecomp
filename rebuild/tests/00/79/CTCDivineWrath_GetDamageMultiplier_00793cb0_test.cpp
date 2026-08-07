#include <cstdio>
struct A { char pad[232]; float* arr; };
struct S { char pad[0xc]; A* f0c; char pad2[80-0x10]; int idx; };
static float f(S* self){ A* a=self->f0c; return a->arr[self->idx]; }
int main(){ float arr[4]={0,0,7.5f,0}; A a; a.arr=arr; S o; o.f0c=&a; o.idx=2;
 if(f(&o)!=7.5f){ std::printf("00793cb0_TEST FAIL\n"); return 1;}
 std::printf("00793cb0_TEST PASS\n"); return 0;}