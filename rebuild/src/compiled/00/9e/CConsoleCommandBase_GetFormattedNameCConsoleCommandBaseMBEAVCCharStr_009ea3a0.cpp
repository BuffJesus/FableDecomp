struct CCharString {
    void* p;
    CCharString() {}
    CCharString(const CCharString& o) { p = o.p; }   // non-trivial -> memory return
};

// helper 0x99f570 as a free __fastcall returning CCharString by value.
CCharString __fastcall sub_99f570(void* field, const char* lit);

struct CConsoleCommandBase {
    void* vtbl;
    char  m_field4;
    CCharString GetFormattedName() const;
};

CCharString CConsoleCommandBase::GetFormattedName() const
{
    return sub_99f570((void*)&m_field4, (const char*)0x13caa48);
}