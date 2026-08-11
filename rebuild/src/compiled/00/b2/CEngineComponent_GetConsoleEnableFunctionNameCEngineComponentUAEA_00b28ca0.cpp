// CEngineComponent::GetConsoleEnableFunctionName
// retail 0x00b28ca0: returns CCharString by value, built from a string literal.
//   push esi; esi=[esp+8](hidden ret buf); push -1; push 0x122d70e; ecx=esi;
//   call CCharString::CCharString(const char*, long); eax=esi; ret 4

class CCharString {
public:
    char* m_p;
    // ctor at 0x99ebf0: CCharString(const char*, long)
    CCharString(const char* s, long n);
};

// Model CEngineComponent with a virtual so the method is virtual (UAE in mangled name).
class CEngineComponent {
public:
    virtual CCharString GetConsoleEnableFunctionName();
};

CCharString CEngineComponent::GetConsoleEnableFunctionName()
{
    return CCharString((const char*)0x122d70e, -1);
}