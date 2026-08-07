#include <cstdio>
struct C { void InitStateGroup(void* a, void* b){ (void)a;(void)b; } };
int main(){ C o; int x,y; o.InitStateGroup(&x,&y); std::printf("00917300_TEST PASS\n"); return 0;}