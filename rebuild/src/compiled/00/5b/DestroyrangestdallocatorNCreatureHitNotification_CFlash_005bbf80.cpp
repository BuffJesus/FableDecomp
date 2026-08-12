#pragma optimize("s",on)
// _Destroy_range<std::allocator<NCreatureHitNotification::CFlash>>
// Retail 0x005bbf80: loop over [first,last) stride 0x14, call element
// dtor helper(this, 0), ret 4.

struct CFlash {
    char pad[0x14];
    void Dtor(int flag);   // __fastcall, one stack arg (push 0)
};

struct AllocTag {};

void __fastcall Destroy_range_flash(CFlash* first, CFlash* last,
                                    AllocTag* al)
{
    for (; first != last; ++first)
        first->Dtor(0);
}