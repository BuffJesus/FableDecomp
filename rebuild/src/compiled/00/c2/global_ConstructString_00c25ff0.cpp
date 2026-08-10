// ConstructString @ 0x00c25ff0
// retail: mov eax,[esp+8]; mov edx,[esp+4]; push eax; push 0; push edx; add ecx,8; call 0xc0b570; ret 8

struct Sub {};

// callee 0xc0b570 : __fastcall  ecx=self, edx=a(register), then stack (a, 0, b) R->L
extern "C" bool __fastcall Sub_Construct(Sub* self, long edx_a, long a_stack, int zero, long b);

struct CWideString {
    char pad[8];
    Sub sub;   // at +8
};

// ecx=self; two stack dwords: [esp+4]=a, [esp+8]=b  -> ret 8
extern "C" bool __fastcall ConstructString(CWideString* self, int /*edx*/, long a, long b)
{
    return Sub_Construct(&self->sub, a, a, 0, b);
}