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