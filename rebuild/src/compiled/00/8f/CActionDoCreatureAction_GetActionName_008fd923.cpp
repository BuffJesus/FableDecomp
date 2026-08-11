#pragma optimize("g",on)

// CCharString ctor at 0x99ebf0: __fastcall member, this in ECX, two stack args
// (const char* lit, int len) cleaned by callee (ret 8). Model as a real member so
// the compiler emits a genuine thiscall call site (ecx=this, no edx touched).
struct CCharString {
    int dummy;
    CCharString* ctor_99ebf0(const char* lit, int len);
};

CCharString* __fastcall GetActionName(CCharString* thisret)
{
    thisret->ctor_99ebf0((const char*)0x1289710, -1);
    return thisret;
}