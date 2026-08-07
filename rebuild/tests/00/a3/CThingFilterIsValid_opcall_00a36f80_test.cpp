#include <cstdio>
struct Filter;
struct CThing { virtual void v0(){} virtual void v1(){} virtual bool V2(Filter* f){ return f!=0; } };
struct Filter { bool op(CThing* t); };
bool Filter::op(CThing* t){ return t->V2(this); }
int main(){ CThing t; Filter f; if(!f.op(&t)){ std::printf("00a36f80_TEST FAIL\n"); return 1;}
 std::printf("00a36f80_TEST PASS\n"); return 0;}