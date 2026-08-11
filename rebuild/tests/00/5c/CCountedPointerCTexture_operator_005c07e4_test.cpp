#include <cstdio>

struct CCountedPointer_CTexture {
    void* m0;
    void* m4;
    void Assign(void* a, void* b);
    CCountedPointer_CTexture* operator=(const CCountedPointer_CTexture& rhs);
};

static void* g_seen_a;
static void* g_seen_b;
static CCountedPointer_CTexture* g_seen_this;

void CCountedPointer_CTexture::Assign(void* a, void* b)
{
    g_seen_this = this;
    g_seen_a = a;
    g_seen_b = b;
    this->m0 = a;
    this->m4 = b;
}

CCountedPointer_CTexture* CCountedPointer_CTexture::operator=(const CCountedPointer_CTexture& rhs)
{
    this->Assign(rhs.m0, rhs.m4);
    return this;
}

int main()
{
    CCountedPointer_CTexture dst;
    dst.m0 = (void*)0x11111111;
    dst.m4 = (void*)0x22222222;

    CCountedPointer_CTexture src;
    src.m0 = (void*)0xAAAA0000;
    src.m4 = (void*)0xBBBB0000;

    CCountedPointer_CTexture* r = (dst = src);

    bool ok = (r == &dst)
        && (g_seen_this == &dst)
        && (g_seen_a == (void*)0xAAAA0000)
        && (g_seen_b == (void*)0xBBBB0000)
        && (dst.m0 == (void*)0xAAAA0000)
        && (dst.m4 == (void*)0xBBBB0000);

    if (ok) {
        printf("COUNTEDPTR_ASSIGN_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}