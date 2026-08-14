#include <cstdio>

struct CTexture;

struct CDisplayManager {
    int marker;
    CTexture* helper(char* a, char* b);
    CTexture* CopyBackBufferToTexture();
};

static int g_helper_calls = 0;
static char* g_last_a = 0;
static char* g_last_b = 0;

CTexture* CDisplayManager::helper(char* a, char* b)
{
    g_helper_calls++;
    g_last_a = a;
    g_last_b = b;
    return (CTexture*)0xDEAD;   // discarded by caller
}

CTexture* CDisplayManager::CopyBackBufferToTexture()
{
    char* pa;
    char* pb;
    { char a; pa = &a; }
    { char b; pb = &b; }
    this->helper(pa, pb);
    return (CTexture*)this;
}

int main()
{
    CDisplayManager dm;
    dm.marker = 7;
    CTexture* r = dm.CopyBackBufferToTexture();
    bool ok = (r == (CTexture*)&dm)      // returns self
           && (g_helper_calls == 1)      // helper called once
           && (g_last_a != 0)            // scratch addresses forwarded
           && (g_last_b != 0);
    printf("%s\n", ok ? "PASS_CopyBackBufferToTexture" : "FAIL");
    return 0;
}