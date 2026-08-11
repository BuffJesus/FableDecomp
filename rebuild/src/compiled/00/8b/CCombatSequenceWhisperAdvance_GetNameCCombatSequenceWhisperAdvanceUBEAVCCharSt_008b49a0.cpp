// CCombatSequenceWhisperAdvance::GetName  (retail 0x008b49a0)
// __fastcall const method, returns CCharString by value (hidden return-buffer ptr).
// Body:  CCharString::CCharString(retbuf, "literal", -1);  return retbuf;
// The original 'this' is never read.

// CCharString::CCharString(const char*, int) @ 0x0099ebf0 is a real __fastcall ctor
// (ecx = this, args const char* and int on the stack, no edx).
struct CCharString {
    char* p;
    CCharString(const char* s, int len);
};

// literal pointer baked in retail as 0x1278d00
static const char* const kName = (const char*)0x1278d00;

struct CCombatSequenceWhisperAdvance {
    int dummy;
    CCharString GetName() const;   // returns by value -> hidden return buffer ptr
};

CCharString CCombatSequenceWhisperAdvance::GetName() const
{
    return CCharString(kName, -1);
}