// 0049d850 _Cons_val_pair_EHeroMorphType_CEntry
// mov ecx,[ecx+0xc]; jmp 0x4498c0  -> tail forwarder

extern void __fastcall sub_4498c0(void* p);

void __fastcall _Cons_val_pair_EHeroMorphType_CEntry(void* self)
{
    sub_4498c0(*(void**)((char*)self + 0xc));
}