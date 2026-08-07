extern char g_vtbl[];
struct Base { void BaseDtor(); };
void __fastcall Dtor(void* self){ *(void**)self = (void*)g_vtbl; ((Base*)((char*)self + 4))->BaseDtor(); }