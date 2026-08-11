// Faithful VC7.1 reconstruction of
//   ?GetConsoleEnableFunctionName@CEnginePrimitiveRenderer@@UAE?AVCCharString@@XZ  (0x00b38c90)
//
// Retail disasm:
//   push esi
//   mov  esi, [esp+8]        ; esi = hidden return buffer (CCharString*)
//   push -1
//   push 0x12a1384           ; const char* literal
//   mov  ecx, esi            ; this = return buffer
//   call CCharString::CCharString(const char*, int)  ; 0x99ebf0
//   mov  eax, esi
//   pop  esi
//   ret  4
//
// A virtual GetConsoleEnableFunctionName() that returns a CCharString by value,
// NRVO'd into the caller-supplied return buffer, initialised from a string
// literal with an explicit length arg of -1.

class CCharString
{
public:
    // Declaration only: defined in another TU (retail 0x99ebf0), so the
    // compiler MUST emit a real call, matching retail's push/push/call.
    CCharString(const char* s, int len);
private:
    char* m_data;
};

class CEnginePrimitiveRenderer
{
public:
    virtual CCharString GetConsoleEnableFunctionName();
};

CCharString CEnginePrimitiveRenderer::GetConsoleEnableFunctionName()
{
    return CCharString("ENABLE_PRIMITIVE_RENDERER", -1);
}