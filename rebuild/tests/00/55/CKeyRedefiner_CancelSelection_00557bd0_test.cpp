#include <cstdio>
static int g_v3=-1,g_v4=-1,g_upd=0; static void* g_rv4=0;
struct Sub { virtual void d0(){} virtual void d1(){} virtual void d2(){} virtual void V3(int e){ g_v3=e; } virtual void V4(int e){ g_v4=e; } };
struct Result { virtual void r0(){} virtual void r1(){} virtual void r2(){} virtual void r3(){} virtual void RV4(void* p){ g_rv4=p; } };
struct S { char pad0[4]; Sub sub; };
static void* g_activeRedefiner=0; static Result g_res;
static void __fastcall UpdateKeyText(S*){ g_upd=1; }
static Result* __fastcall getResult(){ return &g_res; }
static void CancelSelection(S* self){ if(g_activeRedefiner==self){ g_activeRedefiner=0; UpdateKeyText(self); self->sub.V4(0x21); self->sub.V3(0x19); getResult()->RV4(&self->sub); } }
int main(){ S o; g_activeRedefiner=&o; CancelSelection(&o);
 if(g_activeRedefiner||!g_upd||g_v4!=0x21||g_v3!=0x19||g_rv4!=&o.sub){ std::printf("00557bd0_TEST FAIL\n"); return 1;}
 std::printf("00557bd0_TEST PASS\n"); return 0;}