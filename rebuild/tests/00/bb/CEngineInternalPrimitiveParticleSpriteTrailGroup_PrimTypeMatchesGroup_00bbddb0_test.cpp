#include <cstdio>
struct S { bool PrimTypeMatchesGroup(int t); };
bool S::PrimTypeMatchesGroup(int t){ return t == 20; }
int main(){ S o; if(!o.PrimTypeMatchesGroup(20)){ std::printf("00bbddb0_TEST FAIL\n"); return 1;}
 if(o.PrimTypeMatchesGroup(20+1)){ std::printf("00bbddb0_TEST FAIL\n"); return 1;}
 std::printf("00bbddb0_TEST PASS\n"); return 0;}