struct CGenericVertex_LandscapeBg
{
    unsigned char pad[0x44];
    unsigned char flags;

    unsigned int GetU1();
};

unsigned int CGenericVertex_LandscapeBg::GetU1()
{
    unsigned int v = flags;
    v = v >> 2;
    v = v & 1;
    return v;
}