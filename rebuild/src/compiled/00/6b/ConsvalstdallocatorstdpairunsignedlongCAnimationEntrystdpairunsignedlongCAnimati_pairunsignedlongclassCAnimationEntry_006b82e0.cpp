// The subobject at self+0x90 whose member (0x881820) is called with two stack args.
struct Sub {
    void M(void* a, void* b);  // __fastcall: this in ecx, a & b on stack, ret 8
};

void __fastcall _Cons_val_pair_ulong_CAnimationEntry(void* self, int /*edx*/, void* a, void* b)
{
    ((Sub*)((char*)self + 0x90))->M(b, a);
}