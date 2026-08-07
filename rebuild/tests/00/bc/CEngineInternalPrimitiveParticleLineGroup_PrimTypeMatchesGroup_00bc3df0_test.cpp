#include <cstdio>
struct S { bool PrimTypeMatchesGroup(int t); };
bool S::PrimTypeMatchesGroup(int t){ return t == 21; }
int main(){ S o; if(!o.PrimTypeMatchesGroup(21)){ std::printf("00bc3df0_TEST FAIL\n"); return 1;}
 if(o.PrimTypeMatchesGroup(21+1)){ std::printf("00bc3df0_TEST FAIL\n"); return 1;}
 std::printf("00bc3df0_TEST PASS\n"); return 0;}