#include <cstdio>

struct CDataOutputStream
{
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void Write(void* data, int size) = 0;
    void WriteEBOOL(bool value);
};

void CDataOutputStream::WriteEBOOL(bool value)
{
    unsigned char b = value ? 1 : 0;
    this->Write(&b, 1);
}

static unsigned char g_captured = 0xAA;
static int g_size = -1;
static int g_calls = 0;

struct TestStream : CDataOutputStream
{
    void v0() {} void v1() {} void v2() {} void v3() {}
    void Write(void* data, int size)
    {
        g_captured = *(unsigned char*)data;
        g_size = size;
        g_calls++;
    }
};

int main()
{
    TestStream s;
    CDataOutputStream* p = &s;

    p->WriteEBOOL(true);
    if (g_captured != 1 || g_size != 1 || g_calls != 1) { printf("FAIL a\n"); return 1; }

    p->WriteEBOOL(false);
    if (g_captured != 0 || g_size != 1 || g_calls != 2) { printf("FAIL b\n"); return 1; }

    // any nonzero normalizes to 1
    bool weird; *(unsigned char*)&weird = 0x7F;
    p->WriteEBOOL(weird);
    if (g_captured != 1 || g_size != 1 || g_calls != 3) { printf("FAIL c\n"); return 1; }

    printf("WriteEBOOL_OK\n");
    return 0;
}