#include <cstdio>
static int g_seq=0;
struct S { void (__fastcall **vptr)(S*); };
void __fastcall Activate(S* self){ self->vptr[0x14/4](self); self->vptr[0x18/4](self); }
static void __fastcall v14(S* s){ g_seq=g_seq*10+1; }
static void __fastcall v18(S* s){ g_seq=g_seq*10+2; }
int main(){ void(__fastcall* tbl[0x10])(S*)={0}; tbl[0x14/4]=v14; tbl[0x18/4]=v18;
 S o; o.vptr=tbl; Activate(&o);
 if(g_seq!=12){ std::printf("00c100f0_TEST FAIL\n"); return 1;}
 std::printf("00c100f0_TEST PASS\n"); return 0;}