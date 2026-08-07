#include <cstdio>
struct S { bool PrimTypeMatchesGroup(int t); };
bool S::PrimTypeMatchesGroup(int t){ return t == 23; }
int main(){ S o; if(!o.PrimTypeMatchesGroup(23)){ std::printf("00bc2460_TEST FAIL\n"); return 1;}
 if(o.PrimTypeMatchesGroup(23+1)){ std::printf("00bc2460_TEST FAIL\n"); return 1;}
 std::printf("00bc2460_TEST PASS\n"); return 0;}