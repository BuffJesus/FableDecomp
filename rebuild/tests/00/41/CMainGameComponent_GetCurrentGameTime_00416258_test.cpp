#include <cstdio>
struct S; static double __fastcall ggt_helper(S*);
struct S { char pad[0x161a8]; double d161a8; };
static double __fastcall ggt_helper(S*){ return 5.0; }
static double f(S* self){ return ggt_helper(self) - self->d161a8; }
int main(){ S o; o.d161a8=2.0; if(f(&o)!=3.0){ std::printf("00416258_TEST FAIL\n"); return 1;}
 std::printf("00416258_TEST PASS\n"); return 0;}