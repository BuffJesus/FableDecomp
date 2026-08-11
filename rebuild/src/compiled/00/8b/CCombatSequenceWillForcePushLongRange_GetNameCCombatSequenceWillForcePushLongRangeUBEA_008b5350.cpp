// CCombatSequence_WillForcePushLongRange::GetName  @ 008b5350
// __fastcall, returns CCharString by value (hidden return-buffer ptr = first stack arg).
// Body: constructs a CCharString from a string literal with length -1, returns the buffer.

class CCharString {
public:
    // ctor from (const char*, int) — the retail call at 0x99ebf0.
    CCharString(const char* s, int n);
private:
    char* m_p;
};

class CActionDoCreatureAction;

class CCombatSequence_WillForcePushLongRange {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_WillForcePushLongRange::GetName() const
{
    return CCharString("WillForcePushLongRange", -1);
}