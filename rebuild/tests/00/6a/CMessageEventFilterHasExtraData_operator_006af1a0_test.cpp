#include <cstdio>
struct E { char pad[0x3c]; int f3c; };
struct S { int f0; int op(E* e); };
int S::op(E* e){ int d = e->f3c - this->f0; return d == 0; }
int main(){ E e; e.f3c=7; S o; o.f0=7;
 if(o.op(&e)!=1){ std::printf("006af1a0_TEST FAIL\n"); return 1;}
 o.f0=8; if(o.op(&e)!=0){ std::printf("006af1a0_TEST FAIL\n"); return 1;}
 std::printf("006af1a0_TEST PASS\n"); return 0;}