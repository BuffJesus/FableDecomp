#include <cstdio>

struct MemberBuffer {
    int cleared;
    void Clear() { cleared = 0x55; }
};

struct CMemoryDataOutputStream {
    char   pad0[0x3404];
    int    m_size;
    char   pad1[4];
    MemberBuffer m_buffer;
    void Clear();
};

void CMemoryDataOutputStream::Clear()
{
    m_buffer.Clear();
    m_size = 0;
}

int main()
{
    static CMemoryDataOutputStream s;
    s.m_size = 12345;
    s.m_buffer.cleared = 0;
    s.Clear();
    if (s.m_size == 0 && s.m_buffer.cleared == 0x55)
        printf("CLEAR_OK\n");
    else
        printf("CLEAR_FAIL m_size=%d cleared=%d\n", s.m_size, s.m_buffer.cleared);
    return (s.m_size == 0 && s.m_buffer.cleared == 0x55) ? 0 : 1;
}