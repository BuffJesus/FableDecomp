#include <cstdio>
struct S { char pad[0xB8]; void* o; };
static void AddExclusiveObserver(S* self, void* arg){ self->o=arg; }
int main(){ S s; int x; AddExclusiveObserver(&s,&x);
 if(s.o!=&x){ std::printf("005bc4cb_TEST FAIL\n"); return 1;}
 std::printf("005bc4cb_TEST PASS\n"); return 0;}