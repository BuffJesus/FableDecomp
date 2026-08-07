#include <cstdio>
struct P { char pad[0x27]; unsigned char b27; };
struct A { char pad[4]; P* f4; };
struct S { void M(A* a0, char b); };
void S::M(A* a0, char b){ a0->f4->b27 = b; }
int main(){ P p; A a; a.f4=&p; S o; o.M(&a, 5);
 if(p.b27!=5){ std::printf("00b9d650_TEST FAIL\n"); return 1;}
 std::printf("00b9d650_TEST PASS\n"); return 0;}