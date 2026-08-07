#include <cstdio>
struct S { char pad[0xC]; void* f0c; char b10; };
static char f(S* self){ if(!self->f0c) return 0; return self->b10; }
int main(){ S o; o.f0c=0; if(f(&o)!=0){ std::printf("007ea280_TEST FAIL\n"); return 1;}
 int m; o.f0c=&m; o.b10=7; if(f(&o)!=7){ std::printf("007ea280_TEST FAIL\n"); return 1;}
 std::printf("007ea280_TEST PASS\n"); return 0;}