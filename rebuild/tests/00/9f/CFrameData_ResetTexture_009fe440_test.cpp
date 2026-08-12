#include <stdio.h>

struct SubA {
    int x;
    void Reset();
};

struct SubB {
    int x;
    void Clear();
};

struct CFrameData {
    char pad[0x1c];
    SubA a;
    SubB b;
    char pad2[0x38 - 0x24];
    unsigned char f38;
    unsigned char f39;
    unsigned char f3a;
    void ResetTexture();
};

static int g_a_called = 0;
static int g_b_called = 0;

void SubA::Reset() { g_a_called = 1; }
void SubB::Clear() { g_b_called = 1; }

void CFrameData::ResetTexture()
{
    this->b.Clear();
    this->a.Reset();
    this->f38 = 0xff;
    this->f3a = 0x7f;
}

int main()
{
    CFrameData fd;
    fd.f38 = 0; fd.f39 = 0x11; fd.f3a = 0;
    g_a_called = 0; g_b_called = 0;

    fd.ResetTexture();

    int ok = 1;
    if (!g_a_called) ok = 0;
    if (!g_b_called) ok = 0;
    if (fd.f38 != 0xff) ok = 0;
    if (fd.f3a != 0x7f) ok = 0;
    if (fd.f39 != 0x11) ok = 0;  // untouched

    if (ok) printf("RESETTEXTURE_OK\n");
    else printf("RESETTEXTURE_FAIL a=%d b=%d f38=%d f3a=%d f39=%d\n",
                g_a_called, g_b_called, fd.f38, fd.f3a, fd.f39);
    return ok ? 0 : 1;
}