struct CCharString {
    // real member => __fastcall: this in ECX, (lit,-1) pushed and cleaned by callee.
    void ctor(const char* lit, int len);
};

CCharString* __fastcall GetActionName(CCharString* thisret)
{
    thisret->ctor((const char*)0x1288f80, -1);
    return thisret;
}