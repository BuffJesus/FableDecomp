#include <cstdio>
struct S { char pad[0xF]; unsigned char b0f; char pad2[0xC]; unsigned char b1c; };
static unsigned char f(S* self){ if(!self->b0f) return self->b1c; return (unsigned char)0xFF; }
int main(){ S o; o.b0f=0; o.b1c=7; if(f(&o)!=7){ std::printf("00a75c10_TEST FAIL\n"); return 1;}
 o.b0f=1; if(f(&o)!=0xFF){ std::printf("00a75c10_TEST FAIL\n"); return 1;}
 std::printf("00a75c10_TEST PASS\n"); return 0;}