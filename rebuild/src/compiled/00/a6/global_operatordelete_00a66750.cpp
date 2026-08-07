typedef void (__stdcall* Fn)(void*, int, int);
extern Fn g_delete_fn;
void __fastcall operator_delete(void* p){ g_delete_fn(p, 0, 0x8000); }