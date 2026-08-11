// CCombatSequence_BanditAttackSide::GetName  @ 008b3810
// virtual CCharString GetName() const;  (returns by value via hidden buffer)

struct CCharString {
    char* m_data;
    // CCharString::CCharString(char const*, int)  @ 0x0099ebf0  (__fastcall)
    void Init(const char* s, int n);
};

struct CActionDoCreatureAction;

struct CCombatSequence_BanditAttackSide {
    void* vtbl;
    // The hidden return-buffer pointer is the first stack arg.
    CCharString* GetName(CCharString* result);
};

CCharString* CCombatSequence_BanditAttackSide::GetName(CCharString* result)
{
    result->Init((const char*)0x012787b8, -1);
    return result;
}