struct CDrawMiniMap {
    char pad0[0xe9];
    unsigned char f_e9;
    unsigned char pad1;
    unsigned char f_eb;
    unsigned char f_ec;
    char pad2[0x100 - 0xed];
    unsigned char f_100;
    unsigned char IsActive();
};
unsigned char CDrawMiniMap::IsActive()
{
    if (this->f_eb)
        return this->f_100;
    if (this->f_e9 == 0)
        return 0;
    if (this->f_ec == 0)
        return 0;
    return 1;
}
