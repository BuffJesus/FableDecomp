#include <cstdio>
#include <cstring>

// Standalone behaviour model mirroring s.cpp.

struct CCharString {
    char* m_buf;
    CCharString(const CCharString& o) { m_buf = o.m_buf; }
    CCharString() { m_buf = 0; }
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

int main()
{
    CBankFile bf;
    memset(bf.pad, 0, sizeof(bf.pad));
    char storage[] = "BankHandle";
    bf.m_bankHandle.m_buf = storage;

    CCharString r = bf.GetBankHandle(0);

    if (r.m_buf == storage && strcmp(r.m_buf, "BankHandle") == 0) {
        printf("GETBANKHANDLE_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}