// CCombatSequence_TrollRockShuffleBack::GetName  @ 008b4540
// __fastcall, returns CCharString by value (hidden return buffer).
// Body: constructs a CCharString from a constant literal ("...") with len -1.

class CCharString {
public:
    CCharString(const char* s, int len);
    char* m_data;
};

class CCombatSequence_TrollRockShuffleBack {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_TrollRockShuffleBack::GetName() const
{
    return CCharString((const char*)0x1278ba8, -1);
}