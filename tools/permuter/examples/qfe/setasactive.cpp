struct Sub {
    unsigned char f0;
    char pad[0x0c - 1];
    unsigned char fc;
    unsigned char fd;
};
struct CDrawMiniMap {
    char pad0[0xe9];
    unsigned char f_e9;
    char pad1[0x100 - 0xea];
    Sub sub;
    void SetAsActive(unsigned char a);
};
void CDrawMiniMap::SetAsActive(unsigned char a)
{
    this->f_e9 = a;
    if (!a && this->sub.f0) {
        this->sub.fd = 0;
        this->sub.fc = 1;
    }
}
