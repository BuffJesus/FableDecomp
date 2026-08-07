#include <cstdio>
struct S { void* f0; bool op(void* t); };
bool S::op(void* t){ return this->f0 != t; }
int main(){ int m,n; S o; o.f0=&m; if(!o.op(&n)){ std::printf("006a76a0_TEST FAIL\n"); return 1;}
 if(o.op(&m)){ std::printf("006a76a0_TEST FAIL\n"); return 1;}
 std::printf("006a76a0_TEST PASS\n"); return 0;}