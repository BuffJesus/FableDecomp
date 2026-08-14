#pragma optimize("s",on)
// Byte-exact reconstruction of ConstructString @ 0x0066124b
// Retail is __fastcall (ecx=this, all 3 args on stack, ret 0xc). The harness
// rewrites __fastcall->__fastcall, so model as a __fastcall free fn whose first
// param is ecx(this) and whose second param is a dummy that consumes edx (unused
// by the arg list), leaving a,b,c on the stack. It forwards to a helper, passing
// a pointer to a frame-local byte in edx and (a,b,c,0) on the stack, then returns
// this. Frame pointer is kept (retail uses ebp) via optimize("y",off).
#pragma optimize("y", off)

struct CWideString;

// Callee @ 0x6611f7: __fastcall (ecx=this, edx=&byte), stack args a,b,c,0.
extern "C" void __fastcall ConstructString_helper(
    CWideString* self, char* pByte, long a, long b, long c, long zero);

// __fastcall member modelled as __fastcall(self=ecx, edxDummy, a, b, c).
CWideString* __fastcall ConstructString(
    CWideString* self, void* /*edxDummy*/, long a, long b, long c)
{
    char local;
    ConstructString_helper(self, &local, a, b, c, 0);
    return self;
}