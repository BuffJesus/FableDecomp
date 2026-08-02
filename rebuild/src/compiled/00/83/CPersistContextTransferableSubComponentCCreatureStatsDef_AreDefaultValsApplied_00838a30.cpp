struct CCreatureStatsDef
{
    char pad[0x42];
    unsigned char m_flags; // +0x42: bit5 = defaults-applied
};

class CPersistContext_TransferableSubComponent_CCreatureStatsDef_
{
public:
    char pad[0x70];
    CCreatureStatsDef* m_def; // +0x70

    unsigned char __fastcall AreDefaultValsApplied(void* /*edx_unused*/);
};

unsigned char __fastcall CPersistContext_TransferableSubComponent_CCreatureStatsDef_::AreDefaultValsApplied(void* /*edx_unused*/)
{
    unsigned char b = m_def->m_flags;
    b = b >> 5;
    b = b & 1;
    return b;
}