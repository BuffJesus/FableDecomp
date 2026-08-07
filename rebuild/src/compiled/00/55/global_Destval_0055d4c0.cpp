extern void __fastcall dv_h1(void*);
extern void __fastcall dv_h2(void*);
extern void* g_glob;
void __fastcall Dest_val(void* self){ dv_h1(self); dv_h2(g_glob); }