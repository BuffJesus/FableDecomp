// CCombatSequence_BanditKingSpinAround::GetName  @ 008b3c00
// __fastcall, returns CCharString by value (hidden ret-buffer ptr).
// Body: construct CCharString(0x12788d8, -1) into the return buffer, return it.

class CCharString
{
public:
    CCharString(const char* s, int n);
    void* m_data;
};

class CCombatSequence_BanditKingSpinAround
{
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_BanditKingSpinAround::GetName() const
{
    return CCharString((const char*)0x12788d8, -1);
}