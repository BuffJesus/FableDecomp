#include <cstdio>

struct CTexture;

struct CDisplayManager {
    int marker;
    void Helper(char *a, char *b);
    CTexture * CopyBackBufferToTexture();
};

static int g_called = 0;
void CDisplayManager::Helper(char *a, char *b) { g_called = (a != 0 && b != 0) ? 1 : 0; }

#pragma optimize("t",off)
CTexture * CDisplayManager::CopyBackBufferToTexture()
{
    char *pa, *pb;
    { char a; pa = &a; }
    { char b; pb = &b; }
    Helper(pa, pb);
    return (CTexture *)this;
}

int main()
{
    CDisplayManager d;
    d.marker = 42;
    CTexture *r = d.CopyBackBufferToTexture();
    if ((void *)r == (void *)&d && g_called == 1)
        printf("PARITY_OK_004df8cb\n");
    else
        printf("PARITY_FAIL\n");
    return 0;
}