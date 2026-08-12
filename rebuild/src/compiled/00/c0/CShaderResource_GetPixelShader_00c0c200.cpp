// CShaderResource::GetPixelShader  retail 0x00c0c200
// __fastcall; one CCharString* arg on stack; returns 'this'.
//
// Disasm:
//   mov eax,[esp+4]      ; the CCharString* arg
//   push esi; push eax; push eax
//   mov esi,ecx          ; save this
//   call 0xc0bcc0        ; stdcall helper(arg, arg) -> value
//   push eax
//   mov ecx,esi; call 0xc0c060   ; this->consume(value)
//   mov eax,esi; pop esi; ret 4  ; return this

struct CCharString;
struct CPixelShader;

// stdcall free helper: takes two dword args, callee-cleanup.
int __stdcall helper_c0bcc0(CCharString* a, CCharString* b);

struct CShaderResource {
    int consume_c0c060(int v);   // __fastcall member, one arg
    CPixelShader* GetPixelShader(CCharString* name);
};

CPixelShader* CShaderResource::GetPixelShader(CCharString* name)
{
    int v = helper_c0bcc0(name, name);
    this->consume_c0c060(v);
    return reinterpret_cast<CPixelShader*>(this);
}