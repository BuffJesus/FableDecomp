#pragma optimize("s",on)
struct Sub {
    char present;   // +0x00 (this+0x100)
    char pad[0xb];  // +0x01..0x0b
    char f_c;       // +0x0c
    char f_d;       // +0x0d
};

struct CDrawMiniMap {
    char pad0[0xe9];   // 0x00..0xe8
    char active;       // 0xe9
    char pad1[0x16];   // 0xea..0xff
    Sub sub;           // 0x100

    void SetAsActive(bool active);
};

void CDrawMiniMap::SetAsActive(bool active)
{
    this->active = active;
    if (!active) {
        Sub* p = &this->sub;
        if (p->present != 0) {
            p->f_d = 0;
            p->f_c = 1;
        }
    }
}