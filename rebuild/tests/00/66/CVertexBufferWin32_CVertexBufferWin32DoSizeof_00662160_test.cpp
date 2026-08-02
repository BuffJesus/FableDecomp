#include <cstdio>

class CVertexBufferWin32
{
public:
    char pad[0xE0];
    unsigned long m_someSize;

    unsigned long DoSizeof();
};

unsigned long CVertexBufferWin32::DoSizeof()
{
    return m_someSize + 0x10E;
}

int main()
{
    char raw[0xE0 + 4];
    CVertexBufferWin32* obj = reinterpret_cast<CVertexBufferWin32*>(raw);
    obj->m_someSize = 0x1000;

    unsigned long result = obj->DoSizeof();

    if (result == 0x110E)
    {
        printf("PASS_DOSIZEOF_12345\n");
    }
    else
    {
        printf("FAIL got=%lu\n", result);
    }

    return 0;
}