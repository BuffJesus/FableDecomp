typedef void (__stdcall* Fn)(void*);
extern Fn g_pred_fn;
void* __fastcall op_and(void* self){ g_pred_fn(self); return self; }