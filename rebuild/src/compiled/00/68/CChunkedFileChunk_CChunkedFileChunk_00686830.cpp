struct C {};
extern int g_vtbl;
extern void __fastcall base_dtor(C*);
void __fastcall Dtor(C* self){ *(void**)self = &g_vtbl; base_dtor(self); }