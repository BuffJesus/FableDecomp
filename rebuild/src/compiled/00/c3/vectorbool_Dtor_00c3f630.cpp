extern char g_vtbl[];
extern void __fastcall vb_helper(void*);
void __fastcall Dtor(void* self){ *reinterpret_cast<void**>(reinterpret_cast<char*>(self)+0x1f0) = (void*)g_vtbl; vb_helper(self); }