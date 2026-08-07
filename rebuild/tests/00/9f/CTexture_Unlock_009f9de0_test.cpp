#include <cstdio>
static int g_called=0;
struct Inner { void (__stdcall **vtbl)(Inner*, unsigned long); };
struct S { Inner* f0; void Unlock(unsigned long v); };
void S::Unlock(unsigned long v){ Inner* p=this->f0; p->vtbl[0x50/4](p, v); }
static void __stdcall thunk(Inner* p, unsigned long v){ g_called=(int)v; }
int main(){ void(__stdcall* tbl[0x20])(Inner*,unsigned long)={0}; tbl[0x50/4]=thunk;
 Inner innr; innr.vtbl=tbl; S o; o.f0=&innr; o.Unlock(42);
 if(g_called!=42){ std::printf("009f9de0_TEST FAIL\n"); return 1;}
 std::printf("009f9de0_TEST PASS\n"); return 0;}