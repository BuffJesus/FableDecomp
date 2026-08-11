// CBankFile::GetBankHandle  (retail 0x009cbd10)
// CCharString CBankFile::GetBankHandle();  -- returns a copy of the
// member CCharString at this+0xf0 (m_bankHandle). The compiler emits a
// copy-constructor call (retail 0x0099ec30) into the return buffer.

struct CCharString {
    char* m_buf;
    // copy ctor at retail 0x0099ec30
    CCharString(const CCharString& o);
};

struct CBankFile {
    char pad[0xf0];
    CCharString m_bankHandle;

    CCharString GetBankHandle(int unused);
};

CCharString CBankFile::GetBankHandle(int unused)
{
    return this->m_bankHandle;
}