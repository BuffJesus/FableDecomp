#include <cstdio>
struct Inner; typedef void (__fastcall* VFn)(Inner*);
struct Inner { void** vtbl; };
struct Outer { char pad[8]; Inner* inner; };
static int g_hit=0;
static void __fastcall tgt(Inner* p){ (void)p; g_hit=1; }
static void ClearCommands(Outer* self){ Inner* p=self->inner; if(p){ VFn fn=*reinterpret_cast<VFn*>(*reinterpret_cast<char**>(p)+0x28); fn(p); } }
int main(){ void* tbl[64]; for(int i=0;i<64;i++) tbl[i]=0;
 tbl[0x28/4]=(void*)tgt;
 Inner inr; inr.vtbl=(void**)tbl; Outer outr; outr.inner=&inr;
 ClearCommands(&outr);
 if(!g_hit){ std::printf("007e7360_TEST FAIL\n"); return 1;}
 std::printf("007e7360_TEST PASS\n"); return 0;}