// CGenericVar::Init  @ 0x009ea210  __fastcall void(CCharString const&)

class CCharString {
public:
    // out-of-line copy-assignment (real call target 0x99efb0)
    CCharString& operator=(const CCharString& other);
private:
    char* m_data;
};

class CGenericVar {
public:
    void Init(const CCharString& value);
private:
    int   m_pad0;     // +0
    int   m_type;     // +4  set to 5
    CCharString m_value; // +8
};

void CGenericVar::Init(const CCharString& value)
{
    m_value = value;
    m_type = 5;
}