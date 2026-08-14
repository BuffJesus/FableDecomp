#include <cstdio>

static int g_count = -1;
static unsigned char g_byte = 0;
static void* g_self = 0;

struct IDataOutputStream {
    virtual void v0() {}
    virtual void v1() {}
    virtual void v2() {}
    virtual void v3() {}
    virtual void WriteBytes(const void* p, int n) {
        g_self = this;
        g_count = n;
        g_byte = *(const unsigned char*)p;
    }
};

struct CDataOutputStream : public IDataOutputStream {
    void WriteCBYTE(char value, int trailing);
};

void CDataOutputStream::WriteCBYTE(char value, int /*trailing*/)
{
    this->WriteBytes(&value, 8);
}

int main()
{
    CDataOutputStream s;
    s.WriteCBYTE((char)0x5A, 0);

    bool ok = (g_self == (void*)&s) && (g_count == 8) && (g_byte == 0x5A);
    if (ok) printf("PARITY_OK\n");
    else printf("PARITY_FAIL self=%p count=%d byte=%02x\n", g_self, g_count, g_byte);
    return 0;
}