struct GI2 { void** vtbl; };
extern GI2* g_9e9fb0;
typedef char (__fastcall *pf2)(GI2*);
char __fastcall fn_009e9fb0(){ GI2* g=g_9e9fb0; if(g) return ((pf2)g->vtbl[0x18/4])(g); return 0; }