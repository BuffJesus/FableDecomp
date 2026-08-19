struct Pair; struct Alloc;
extern void __fastcall cons_construct(void* dst, Pair* val);
void __fastcall Cons_val(Alloc* self, Pair* dst, Pair* val)
{
    (void)dst;
    cons_construct(*reinterpret_cast<void**>(self), val);
}