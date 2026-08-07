#include <cstdio>
struct C { int AddChildPrimitive(){ return 0xf; } };
int main(){ C o; if(o.AddChildPrimitive()!=0xf){ std::printf("007b2990_TEST FAIL\n"); return 1;} std::printf("007b2990_TEST PASS\n"); return 0;}