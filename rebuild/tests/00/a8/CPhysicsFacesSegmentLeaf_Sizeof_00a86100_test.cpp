#include <cstdio>
struct S { char pad[4]; short* begin; char gap[4]; short* end; };
static int Sizeof(S* self){ int n=(int)(self->end - self->begin); return n*2+12; }
int main(){ short buf[8]; S obj; obj.begin=buf; obj.end=buf+5;
 if(Sizeof(&obj)!=22){ std::printf("00a86100_TEST FAIL got %d\n", Sizeof(&obj)); return 1;}
 std::printf("00a86100_TEST PASS\n"); return 0;}