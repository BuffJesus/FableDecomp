struct CCharString {
    char* p;
    /* real thiscall member at 0x99ebf0: this=ecx, (const char*, int) on stack */
    void Init(const char* s, int n);
};

/* value-returning method: ecx = hidden return-object pointer (esi in retail) */
CCharString* __fastcall FableGetActionName_5(CCharString* retbuf)
{
    retbuf->Init((const char*)0x123bb40, -1);
    return retbuf;
}