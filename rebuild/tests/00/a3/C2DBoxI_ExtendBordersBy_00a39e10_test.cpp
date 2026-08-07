#include <cstdio>
struct C2DBoxI { int a,b,c,d; void ExtendBordersBy(long); };
void C2DBoxI::ExtendBordersBy(long){ --a; ++c; --b; ++d; }
int main(){ C2DBoxI o; o.a=o.b=o.c=o.d=10; o.ExtendBordersBy(5);
 if(o.a!=9||o.b!=9||o.c!=11||o.d!=11){ std::printf("00a39e10_TEST FAIL\n"); return 1;}
 std::printf("00a39e10_TEST PASS\n"); return 0;}