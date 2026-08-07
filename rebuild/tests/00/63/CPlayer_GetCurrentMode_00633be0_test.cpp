#include <cstdio>
struct C3 { char pad[8]; int mode; };
struct B2 { char pad[4]; C3* c; };
struct A1 { char pad[0x210]; B2* b; };
static int GetCurrentMode(A1* self){ return self->b->c->mode; }
int main(){ C3 c; c.mode=77; B2 b; b.c=&c; A1 a; a.b=&b;
 if(GetCurrentMode(&a)!=77){ std::printf("00633be0_TEST FAIL\n"); return 1;}
 std::printf("00633be0_TEST PASS\n"); return 0;}