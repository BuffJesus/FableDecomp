#include <cstdio>

struct CCreatureStatsDefT
{
    char pad[0x42];
    unsigned char m_flags;
};

class CPersistContext_TransferableSubComponent_CCreatureStatsDef_T
{
public:
    char pad[0x70];
    CCreatureStatsDefT* m_def;

    unsigned char __fastcall AreDefaultValsApplied(void*);
};

unsigned char __fastcall CPersistContext_TransferableSubComponent_CCreatureStatsDef_T::AreDefaultValsApplied(void*)
{
    unsigned char b = m_def->m_flags;
    b = b >> 5;
    b = b & 1;
    return b;
}

int main()
{
    CCreatureStatsDefT def;
    def.m_flags = 0x20; // bit5 set

    CPersistContext_TransferableSubComponent_CCreatureStatsDef_T ctx;
    ctx.m_def = &def;

    unsigned char r1 = ctx.AreDefaultValsApplied(0);
    if (r1 != 1) { return 1; }

    def.m_flags = 0x00;
    unsigned char r2 = ctx.AreDefaultValsApplied(0);
    if (r2 != 0) { return 2; }

    def.m_flags = 0xFF;
    unsigned char r3 = ctx.AreDefaultValsApplied(0);
    if (r3 != 1) { return 3; }

    printf("AREDEFAULTVALSAPPLIED_OK\n");
    return 0;
}