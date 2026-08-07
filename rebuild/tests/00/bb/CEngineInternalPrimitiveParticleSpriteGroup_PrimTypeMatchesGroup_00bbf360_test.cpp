#include <cstdio>
struct S { bool PrimTypeMatchesGroup(int t); };
bool S::PrimTypeMatchesGroup(int t){ return t == 22; }
int main(){ S o; if(!o.PrimTypeMatchesGroup(22)){ std::printf("00bbf360_TEST FAIL\n"); return 1;}
 if(o.PrimTypeMatchesGroup(22+1)){ std::printf("00bbf360_TEST FAIL\n"); return 1;}
 std::printf("00bbf360_TEST PASS\n"); return 0;}