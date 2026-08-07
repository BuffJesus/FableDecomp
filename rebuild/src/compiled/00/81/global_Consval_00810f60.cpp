struct Pair; struct Alloc;
extern void __fastcall cons_helper(void*);
void __fastcall Cons_val(Alloc* self, Pair* dst, char* val){ (void)self; (void)dst; cons_helper(*reinterpret_cast<void**>(val + 8)); }