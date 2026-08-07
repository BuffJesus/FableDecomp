#include <cstdio>
static int g_ev1=-1,g_ev2=-1; static float g_f=-1;
struct Sub { virtual void d0(){} virtual void d1(){} virtual void d2(){} virtual void d3(){} virtual void V4(int e){ if(g_ev1<0) g_ev1=e; else g_ev2=e; } };
struct S { char pad0[4]; Sub sub; char pad1[0x174]; unsigned char b17c; unsigned char b17d; char pad2[2]; float f180; };
static void __fastcall od_helper(S* p, float f){ (void)p; g_f=f; }
static void OnDeactivate(S* self){ self->b17c=0; od_helper(self,0.2f); self->f180=0.2f; self->b17d=0; self->sub.V4(0x25); self->sub.V4(0x24); }
int main(){ S o; o.b17c=9;o.b17d=9; OnDeactivate(&o);
 if(o.b17c||o.b17d||o.f180!=0.2f||g_f!=0.2f||g_ev1!=0x25||g_ev2!=0x24){ std::printf("00556580_TEST FAIL\n"); return 1;}
 std::printf("00556580_TEST PASS\n"); return 0;}