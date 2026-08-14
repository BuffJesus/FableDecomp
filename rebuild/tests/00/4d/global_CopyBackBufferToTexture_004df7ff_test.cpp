#include <cstdio>

struct CTexture;

struct CDisplayManager {
    int tag;
    void Grab(char* a, char* b);
    CTexture* CopyBackBufferToTexture();
};

static char g_seen_two = 0, g_seen_this = 0;

void CDisplayManager::Grab(char* a, char* b)
{
    if (a != 0 && b != 0) g_seen_two = 1;
    if (this != 0) g_seen_this = 1;
}

CTexture* CDisplayManager::CopyBackBufferToTexture()
{
    char* p;
    char* q;
    { char a; p = &a; }
    { char b; q = &b; }
    Grab(p, q);
    return (CTexture*)this;
}

int main()
{
    CDisplayManager dm;
    dm.tag = 7;
    CTexture* r = dm.CopyBackBufferToTexture();
    if ((void*)r == (void*)&dm && g_seen_two && g_seen_this)
        printf("PARITY_OK_004df7ff\n");
    else
        printf("PARITY_FAIL\n");
    return 0;
}