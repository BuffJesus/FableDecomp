#include <cstdio>
struct CTexture;

struct CDisplayManager {
    int calls;
    char *seen_a;
    char *seen_b;
    CTexture *InitContainerSlot(char *a, char *b);
    CTexture *CopyBackBufferToTexture();
};

CTexture *CDisplayManager::InitContainerSlot(char *a, char *b) {
    calls++;
    seen_a = a;
    seen_b = b;
    return reinterpret_cast<CTexture *>(this);
}

#pragma optimize("s", on)
CTexture *CDisplayManager::CopyBackBufferToTexture()
{
    char *pa;
    char *pb;
    { char a; pa = &a; }
    { char b; pb = &b; }
    InitContainerSlot(pa, pb);
    return reinterpret_cast<CTexture *>(this);
}

int main()
{
    CDisplayManager d;
    d.calls = 0;
    d.seen_a = 0;
    d.seen_b = 0;
    CTexture *r = d.CopyBackBufferToTexture();
    bool ok = (r == reinterpret_cast<CTexture *>(&d))
           && (d.calls == 1)
           && (d.seen_a != 0)
           && (d.seen_b != 0);
    printf(ok ? "PARITY_OK\n" : "PARITY_FAIL\n");
    return 0;
}