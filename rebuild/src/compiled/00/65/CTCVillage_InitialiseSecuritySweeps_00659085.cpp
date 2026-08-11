#pragma optimize("s",on)
// CTCVillage::InitialiseSecuritySweeps @ 0x00659085

struct Sweeps
{
    int dummy;
    void Reset();  // 0x65fefc, non-inlined method on member at +0x5c
};

struct CTCVillage
{
    char  pad00[0x10];    // +0x00
    int   m_field10;      // +0x10
    char  pad14[0x5c-0x14];  // +0x14 .. +0x5b
    Sweeps m_sweeps;      // +0x5c
    char  pad60[0xb8-0x60];  // +0x60 .. +0xb7
    float m_field_b8;     // +0xb8
    void InitialiseSecuritySweeps();
};

void CTCVillage::InitialiseSecuritySweeps()
{
    m_sweeps.Reset();
    m_field10 = 0;
    m_field_b8 = 20.0f;
}