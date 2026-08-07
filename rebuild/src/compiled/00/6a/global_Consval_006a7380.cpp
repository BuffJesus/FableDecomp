extern void __fastcall cons_helper(void* self, void* dst, char* v);
void __fastcall Cons_val(void* self, void* dst, char* val){ cons_helper(self, dst, val + 0xc); }