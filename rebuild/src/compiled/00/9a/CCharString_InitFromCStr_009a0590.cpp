struct CCharString
{
    char* m_data;
    long m_length;
    unsigned long m_flags;
    unsigned char m_flagByte;

    void AssignFromData(const char* data, int len);

    CCharString* InitFromCStr(const char* s);
};

CCharString* CCharString::InitFromCStr(const char* s)
{
    m_flags = m_flags & 0x80000000;
    m_flagByte = m_flagByte | 1;
    m_data = 0;
    m_length = 0;

    const char* p = s;
    int len = 0;
    if (*p != 0)
    {
        do
        {
            ++p;
            ++len;
        } while (*p != 0);
    }

    AssignFromData(s, len);
    return this;
}