// CCombatSequenceWhisperBoast::GetName
// Retail 0x008b4ca0 : virtual CCharString GetName() const  (returns by value via ecx=ret buffer)
//   push esi ; esi = hidden return-buffer ptr [esp+8]
//   push -1  ; push 0x1278dc4 ; ecx = esi ; call CCharString::CCharString(const char*, int)
//   eax = esi ; ret 4
//
// CCharString has non-trivial copy semantics, so it is returned via a hidden
// return-buffer pointer (passed as the first stack arg).  NRVO constructs the
// named local directly in that buffer, so the whole body is one in-place ctor.

struct CCharString
{
    char* m_pData;
    int   m_nLength;

    CCharString(const char* s, int len);   // retail 0x0099ebf0
    CCharString(const CCharString& o);      // non-trivial copy => memory return
    ~CCharString();
};

struct CCombatSequenceWhisperBoast
{
    virtual CCharString GetName() const;
};

CCharString CCombatSequenceWhisperBoast::GetName() const
{
    return CCharString(reinterpret_cast<const char*>(0x1278dc4), -1);
}