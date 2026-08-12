// Retail 0x006334f0
// void __fastcall _Dest_val(Owner* this, allocator*, pair*)
// push esi; mov esi,ecx; mov ecx,[esi+0x140]; call 0xa0d2d0;
// mov ecx,[esi+0x1a4]; call 0xa0d2d0; pop esi; ret 8

struct Sub {
    int pad;
    void Helper();   // 0xa0d2d0 (__fastcall)
};

struct Owner {
    char pad[0x140];
    Sub* m_a;        // +0x140
    char pad2[0x1a4 - 0x140 - 4];
    Sub* m_b;        // +0x1a4
    void _Dest_val_helper(void* al, void* pr);
};

void Owner::_Dest_val_helper(void* al, void* pr)
{
    m_a->Helper();
    m_b->Helper();
}