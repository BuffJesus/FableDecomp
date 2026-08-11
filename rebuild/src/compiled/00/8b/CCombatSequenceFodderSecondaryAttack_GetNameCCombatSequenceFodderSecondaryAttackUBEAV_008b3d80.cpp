// CCombatSequence_FodderSecondaryAttack::GetName  (retail 0x008b3d80)
// virtual CCharString GetName() const;  -- returns a fixed literal name.

struct CCharString {
    char* m_buf;
    // ctor at retail 0x0099ebf0: CCharString(const char* s, int len)
    CCharString(const char* s, int len);
};

// The literal at retail 0x01278954.
static const char kName[] = "FodderSecondaryAttack";

struct CCombatSequence_FodderSecondaryAttack {
    void* vtbl;
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_FodderSecondaryAttack::GetName() const
{
    return CCharString(kName, -1);
}