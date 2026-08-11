// CEnginePrimitiveRenderer2D::GetConsoleEnableFunctionName
// Retail 0x00b4b6f0 (__fastcall, returns CCharString by value).
//
// Body just constructs a constant CCharString("...", -1) into the hidden
// return buffer and returns it. The `this` pointer is unused.

// Minimal CCharString: a single data pointer, ctor(const char*, int).
// The ctor is an external engine routine (retail 0x0099ebf0). We only need
// its declaration so VC7.1 emits the push -1 / push <lit> / mov ecx / call
// sequence; the call target + literal address are reloc slots the harness masks.
class CCharString
{
public:
    CCharString(const char* s, int n);
    char* m_data;
};

class CEnginePrimitiveRenderer2D
{
public:
    virtual CCharString GetConsoleEnableFunctionName();
};

CCharString CEnginePrimitiveRenderer2D::GetConsoleEnableFunctionName()
{
    return CCharString("ConsoleEnable", -1);
}