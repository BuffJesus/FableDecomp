#include <cstdio>
struct S { int f0; bool IsEquivalent(S* other); };
bool S::IsEquivalent(S* other){ return this->f0 == other->f0; }
int main(){ S a; a.f0=5; S b; b.f0=5; if(!a.IsEquivalent(&b)){ std::printf("009e37f0_TEST FAIL\n"); return 1;}
 b.f0=6; if(a.IsEquivalent(&b)){ std::printf("009e37f0_TEST FAIL\n"); return 1;}
 std::printf("009e37f0_TEST PASS\n"); return 0;}