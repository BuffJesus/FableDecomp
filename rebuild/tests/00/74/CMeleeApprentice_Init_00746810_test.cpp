#include <cstdio>
struct S { char pad[0x0C]; unsigned char a; unsigned char b; unsigned char c; };
static void Init(S* self){ self->a=1; self->b=0; self->c=0; }
int main(){ S obj; obj.a=9; obj.b=9; obj.c=9; Init(&obj);
 if(obj.a!=1||obj.b!=0||obj.c!=0){ std::printf("00746810_TEST FAIL\n"); return 1;}
 std::printf("00746810_TEST PASS\n"); return 0;}