#pragma optimize("g",on)
struct CCharString {
    int dummy;
    // ctor at 0x99ebf0: this in ECX, two stack args (lit, len), callee cleanup (native thiscall).
    CCharString* ctor(const char* lit, int len);
};

CCharString* __fastcall GetActionName(CCharString* thisret)
{
    thisret->ctor((const char*)0x1288ff0, -1);
    return thisret;
}