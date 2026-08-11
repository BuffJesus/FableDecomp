// CTCGraphicAppearance::SetAlpha  @ 004bfac0
struct Sub {
    void Touch();  // non-virtual thiscall @ 0x4c7ac0, no args
};

struct CTCGraphicAppearance {
    char pad0[4];        // +0x00
    Sub* f4;             // +0x04
    char pad8[0x58 - 8]; // +0x08 .. +0x57
    unsigned char flags; // +0x58
    char pad59;          // +0x59
    unsigned char alpha; // +0x5a
    void SetAlpha(unsigned char a);
};

void CTCGraphicAppearance::SetAlpha(unsigned char a)
{
    this->alpha = a;
    this->f4->Touch();
    this->flags |= 4;
}