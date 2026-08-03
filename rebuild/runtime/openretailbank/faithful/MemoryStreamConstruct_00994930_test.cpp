#include <cstdio>

struct CDataInputStream
{
    virtual ~CDataInputStream() {}

    CDataInputStream() {}
};

struct CMemoryDataInputStream : CDataInputStream
{
    unsigned long m_field04;
    unsigned long m_field08;
    unsigned long m_field0c;
    unsigned long m_field10;
    unsigned long m_field14;
    unsigned long m_field18;

    CMemoryDataInputStream(void* buf, unsigned long size);
};

CMemoryDataInputStream::CMemoryDataInputStream(void* buf, unsigned long size)
    : CDataInputStream(),
      m_field08(size),
      m_field14(size),
      m_field04(0),
      m_field0c((unsigned long)buf),
      m_field10(0),
      m_field18((unsigned long)buf)
{
}

int main()
{
    void* fakebuf = (void*)0x12345678;
    unsigned long fakesize = 0xABCD;

    CMemoryDataInputStream stream(fakebuf, fakesize);

    int ok = 1;
    if (stream.m_field04 != 0) ok = 0;
    if (stream.m_field08 != fakesize) ok = 0;
    if (stream.m_field0c != (unsigned long)fakebuf) ok = 0;
    if (stream.m_field10 != 0) ok = 0;
    if (stream.m_field14 != fakesize) ok = 0;
    if (stream.m_field18 != (unsigned long)fakebuf) ok = 0;

    if (ok)
        printf("TEST_OK_CMemoryDataInputStream_ctor\n");
    else
        printf("TEST_FAIL\n");

    return 0;
}
