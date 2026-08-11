// CCombatSequence_TrollBurrowPursue::GetName  (retail 0x008b4200)
// virtual CCharString GetName() const;  -- returns a fixed literal name.

struct CCharString {
    char* m_buf;
    // ctor at retail 0x0099ebf0: CCharString(const char* s, int len)
    CCharString(const char* s, int len);
};

// The literal at retail 0x01278a9c.
static const char kName[] = "TrollBurrowPursue";

struct CCombatSequence_TrollBurrowPursue {
    void* vtbl;
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_TrollBurrowPursue::GetName() const
{
    return CCharString(kName, -1);
}