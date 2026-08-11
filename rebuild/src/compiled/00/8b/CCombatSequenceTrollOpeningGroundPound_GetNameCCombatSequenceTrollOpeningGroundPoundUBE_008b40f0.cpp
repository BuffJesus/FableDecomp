// CCombatSequence_TrollOpeningGroundPound::GetName  @ 008b40f0
// virtual public const, returns CCharString by value.
// Body constructs the return CCharString in place (NRVO into the hidden
// return buffer) from a fixed global literal pointer with length -1.
//
// CCharString(const char*, int) ctor lives at 0x0099ebf0.  'this' (ecx of
// GetName) is ignored by the body.

struct CCharString {
    void* p;
    CCharString(const char* s, int len);   // 0x0099ebf0
};

struct CCombatSequence_TrollOpeningGroundPound {
    CCharString GetName() const;
};

CCharString CCombatSequence_TrollOpeningGroundPound::GetName() const
{
    return CCharString((const char*)0x1278a48, -1);
}