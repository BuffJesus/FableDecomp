#include <cstdio>
struct S { int f0; int f4; char M(); };
char S::M(){ if(this->f4 >= 0) return 1; return this->f0 == -1; }
int main(){ S o; o.f4=5; if(o.M()!=1){ std::printf("00733340_TEST FAIL\n"); return 1;}
 o.f4=-1; o.f0=3; if(o.M()!=0){ std::printf("00733340_TEST FAIL\n"); return 1;}
 o.f0=-1; if(o.M()!=1){ std::printf("00733340_TEST FAIL\n"); return 1;}
 std::printf("00733340_TEST PASS\n"); return 0;}