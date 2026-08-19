struct That2 { void* m(int a); };
struct GB { char pad[0x6b0]; That2* f6b0; };
extern GB* g_00bf36a0;
void* __fastcall fn_00bf36a0(int arg){ GB* g=g_00bf36a0; return g->f6b0->m(arg); }