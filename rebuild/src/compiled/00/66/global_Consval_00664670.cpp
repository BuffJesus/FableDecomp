struct Pair; struct Alloc;
extern void __fastcall cons_helper(Alloc*, Pair*, void*);
void __fastcall Cons_val(Alloc* self, Pair* dst, void** val){ cons_helper(self, dst, *val); }