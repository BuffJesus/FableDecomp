#include <cstdio>
struct S; typedef S* (__fastcall* VFn)(S*);
struct S { VFn* vtbl; char pad[4]; void* f8; };
static S g_new;
static S* __fastcall vm(S* p){ (void)p; return &g_new; }
static S* f(S* self){ S* p=self->vtbl[0x10/4](self); p->f8=self->f8; return p; }
int main(){ VFn t[8]; for(int i=0;i<8;i++)t[i]=0; t[0x10/4]=vm;
 int m; S o; o.vtbl=t; o.f8=&m; S* r=f(&o);
 if(r!=&g_new||r->f8!=&m){ std::printf("005db460_TEST FAIL\n"); return 1;}
 std::printf("005db460_TEST PASS\n"); return 0;}