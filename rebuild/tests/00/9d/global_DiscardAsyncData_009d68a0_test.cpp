#include <cstdio>
static void* g_arg=0;
struct SubObj { void M(void* arg); };
void SubObj::M(void* arg){ g_arg=arg; }
struct P { char pad[0x28]; SubObj* f28; };
struct S { P* f0; };
static void DiscardAsyncData(S* self){ P* p=self->f0; if(p) p->f28->M(self); }
int main(){ SubObj so; P p; p.f28=&so; S o; o.f0=&p; DiscardAsyncData(&o);
 if(g_arg!=&o){ std::printf("009d68a0_TEST FAIL\n"); return 1;}
 S z; z.f0=0; DiscardAsyncData(&z);
 std::printf("009d68a0_TEST PASS\n"); return 0;}