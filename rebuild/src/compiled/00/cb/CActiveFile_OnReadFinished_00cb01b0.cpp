typedef void (__stdcall* Fn)(void*);
extern Fn g_orf_fn;
struct S { void* p; };
void __fastcall OnReadFinished(S* self){ if(self->p) g_orf_fn(self->p); }