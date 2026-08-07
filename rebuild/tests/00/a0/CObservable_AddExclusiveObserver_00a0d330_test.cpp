#include <cstdio>
struct S { char pad[0xAC]; void* o; };
static void AddExclusiveObserver(S* self, void* arg){ self->o=arg; }
int main(){ S obj; int marker; AddExclusiveObserver(&obj,&marker);
 if(obj.o!=&marker){ std::printf("00a0d330_TEST FAIL\n"); return 1;}
 std::printf("00a0d330_TEST PASS\n"); return 0;}