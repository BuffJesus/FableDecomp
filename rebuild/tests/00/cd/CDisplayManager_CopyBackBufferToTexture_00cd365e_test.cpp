#include <stdio.h>

struct CTexture;
struct CDisplayManager {
    int dummy;
    void Helper(char* a, char* b);
    CTexture* CopyBackBufferToTexture();
};

static int   g_called = 0;
static char* g_a = 0;
static char* g_b = 0;

void CDisplayManager::Helper(char* a, char* b)
{
    g_called = 1;
    g_a = a;
    g_b = b;
}

CTexture* CDisplayManager::CopyBackBufferToTexture()
{
    char local;
    this->Helper(&local, &local + 1 - 1);
    return (CTexture*)this;
}

int main()
{
    CDisplayManager dm;
    dm.dummy = 42;
    CTexture* r = dm.CopyBackBufferToTexture();
    if (r == (CTexture*)&dm && g_called == 1 && g_a == g_b && g_a != 0) {
        printf("PARITY_OK\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}