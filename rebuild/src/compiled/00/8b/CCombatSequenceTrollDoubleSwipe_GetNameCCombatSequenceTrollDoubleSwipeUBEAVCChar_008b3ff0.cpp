// CCombatSequence_TrollDoubleSwipe::GetName -> CCharString by value
// retail 0x008b3ff0
//
// push esi
// mov esi,[esp+8]        ; esi = hidden return-buffer ptr
// push -1
// push 0x12789f4         ; string literal
// mov ecx,esi
// call CCharString::CCharString(char const*, int)   @0x99ebf0
// mov eax,esi
// pop esi
// ret 4

class CCharString
{
public:
    CCharString(const char* s, int n);
private:
    void* m_data;
};

class CCombatSequence_TrollDoubleSwipe
{
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_TrollDoubleSwipe::GetName() const
{
    return CCharString("TrollDoubleSwipe", -1);
}