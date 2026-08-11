#include <stdio.h>

struct Sweeps
{
    int dummy;
    void Reset();
};

struct CTCVillage
{
    char  pad00[0x10];
    int   m_field10;
    char  pad14[0x5c-0x14];
    Sweeps m_sweeps;
    char  pad60[0xb8-0x60];
    float m_field_b8;
    void InitialiseSecuritySweeps();
};

static int g_resetCalled = 0;

void Sweeps::Reset()
{
    g_resetCalled = 1;
    this->dummy = 0x1234;
}

void CTCVillage::InitialiseSecuritySweeps()
{
    m_sweeps.Reset();
    m_field10 = 0;
    m_field_b8 = 20.0f;
}

int main()
{
    CTCVillage v;
    v.m_field10 = 0x7777;
    v.m_field_b8 = -1.0f;
    v.m_sweeps.dummy = 0;
    v.InitialiseSecuritySweeps();

    int ok = 1;
    if (g_resetCalled != 1) ok = 0;
    if (v.m_sweeps.dummy != 0x1234) ok = 0;
    if (v.m_field10 != 0) ok = 0;
    if (v.m_field_b8 != 20.0f) ok = 0;

    if (ok) printf("SWEEPS_INIT_OK\n");
    else    printf("SWEEPS_INIT_FAIL\n");
    return ok ? 0 : 1;
}