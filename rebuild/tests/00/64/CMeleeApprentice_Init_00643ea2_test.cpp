#include <cstdio>
struct S { char pad[0x144]; unsigned char b144; unsigned char b145; };
static void f(S* self){ self->b145=0; self->b144=1; }
int main(){ S o; o.b144=9;o.b145=9; f(&o); if(o.b144!=1||o.b145!=0){ std::printf("00643ea2_TEST FAIL\n"); return 1;}
 std::printf("00643ea2_TEST PASS\n"); return 0;}