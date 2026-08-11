// CCombatSequence_BanditIdleMiddle::GetName  (retail 0x008b3980)
// virtual const method returning CCharString by value; body is just
//   return CCharString("literal", -1);

class CCharString
{
public:
    CCharString(const char* str, long len);
private:
    char* m_pData;
    unsigned long m_Length;
    unsigned long m_Capacity;
};

// retail ctor at 0x0099ebf0: CCharString(CCharString*, char*, long)
extern const char s_BanditIdleMiddleName[];

class CCombatSequence_BanditIdleMiddle
{
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_BanditIdleMiddle::GetName() const
{
    return CCharString(s_BanditIdleMiddleName, -1);
}