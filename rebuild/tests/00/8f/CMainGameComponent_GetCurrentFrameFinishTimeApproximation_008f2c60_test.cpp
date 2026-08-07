#include <cstdio>
struct S; static double __fastcall gcf_helper(S*);
struct S { char pad[0x118]; float f118; };
static double __fastcall gcf_helper(S*){ return 3.0; }
static double f(S* self){ return gcf_helper(self) * self->f118; }
int main(){ S o; o.f118=2.0f; if(f(&o)!=6.0){ std::printf("008f2c60_TEST FAIL\n"); return 1;}
 std::printf("008f2c60_TEST PASS\n"); return 0;}