// Retail: mov ecx,[ecx+0x2c]; jmp target  (tail call, __fastcall)
// void __fastcall f(void* self){ t(*(void**)((char*)self+0x2c)); }

extern void __fastcall _Cons_val_tail_00f35a10(void* self);

void __fastcall _Cons_val_pair_EHeroMorphType_CEntry_v2(void* self)
{
    _Cons_val_tail_00f35a10(*(void**)((char*)self + 0x2c));
}