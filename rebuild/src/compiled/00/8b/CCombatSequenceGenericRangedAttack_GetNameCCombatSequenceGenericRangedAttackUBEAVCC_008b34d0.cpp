// CCombatSequence_GenericRangedAttack::GetName  (retail 0x008b34d0)
// virtual CCharString GetName() const;  -- returns a fixed literal name.

struct CCharString {
    char* m_buf;
    // ctor at retail 0x0099ebf0: CCharString(const char* s, int len)
    CCharString(const char* s, int len);
};

// The literal at retail 0x012786ac.
static const char kName[] = "GenericRangedAttack";

struct CCombatSequence_GenericRangedAttack {
    void* vtbl;
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_GenericRangedAttack::GetName() const
{
    return CCharString(kName, -1);
}