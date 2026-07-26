struct C2DBoxF { float a, b, c, d; };

struct CEditControlCentre {
    char pad0[0x28c];
    unsigned char flagB;   // +0x28c
    char pad1[0x294 - 0x28d];
    unsigned char flagA;   // +0x294

    void h1(int p1, int p2, int p4);
    void h2(int p3, int p4, C2DBoxF* box);
    void h3(int p4, C2DBoxF* box);
    void h4(int p3, int p4);
    void h5(int p3, int p4);
    bool GetCurrentMask(int p1, int p2, int p3, int p4);
};

// CEditControlCentre::GetCurrentMask
bool CEditControlCentre::GetCurrentMask(int p1, int p2, int p3, int p4)
{
    __declspec(align(16)) char storage[0x1f4];
    C2DBoxF* box = (C2DBoxF*)storage;
    if (!this->flagA)
        return false;
    h1(p1, p2, p4);
    h2(p3, p4, box);
    h3(p4, box);
    h4(p3, p4);
    h5(p3, p4);
    this->flagB = (unsigned char)p2;
    return true;
}