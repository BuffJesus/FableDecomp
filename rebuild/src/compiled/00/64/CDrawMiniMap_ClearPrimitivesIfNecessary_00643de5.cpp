#pragma optimize("s",on)

struct SubObj { void Clear(); };  // helper at 0x82a1d0 (thiscall, no args)
struct CDrawMiniMap {
    virtual void vslot0();     // [vptr+0]
    virtual void vslot4();     // [vptr+4]
    char pad0[0x18 - 4];
    unsigned char f_18;   // +0x18
    char pad1[0x28 - 0x19];
    SubObj sub;           // +0x28
    char pad2[0xeb - 0x29];
    unsigned char f_eb;   // +0xeb
    void ClearPrimitivesIfNecessary();
};
void CDrawMiniMap::ClearPrimitivesIfNecessary() {
    if (this->f_18 == 0)
        this->sub.Clear();
    if (this->f_eb == 0)
        this->vslot4();
}