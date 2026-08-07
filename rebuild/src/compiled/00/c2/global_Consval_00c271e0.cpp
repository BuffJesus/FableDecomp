struct Pair; struct Alloc;
extern double __fastcall cons_ctor(Pair* val);
void __fastcall Cons_val(Alloc* self, Pair* dst, Pair* val){ (void)self; (void)dst; cons_ctor(val); }