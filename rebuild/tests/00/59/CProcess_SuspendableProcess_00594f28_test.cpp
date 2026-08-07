#include <cstdio>
struct S; typedef void (__fastcall* VFn)(S*);
struct S { void** vtbl; char pad[0x25]; unsigned char b29; };
static int g_ran=0;
static void __fastcall vmethod(S* p){ (void)p; g_ran=1; }
static void SuspendableProcess(S* self){ VFn fn=reinterpret_cast<VFn>(self->vtbl[0x10/4]); fn(self); self->b29=1; }
int main(){ void* tbl[16]; for(int i=0;i<16;i++) tbl[i]=0; tbl[0x10/4]=(void*)vmethod;
 S obj; obj.vtbl=(void**)tbl; obj.b29=0; SuspendableProcess(&obj);
 if(!g_ran||obj.b29!=1){ std::printf("00594f28_TEST FAIL\n"); return 1;}
 std::printf("00594f28_TEST PASS\n"); return 0;}