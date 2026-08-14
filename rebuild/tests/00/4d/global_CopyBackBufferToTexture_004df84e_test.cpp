#include <cstdio>

struct CTexture;

struct CDisplayManager {
    int tag;
    CTexture& CopyBackBufferToTexture();
    void Handler(char* a, char* b);
};

void CDisplayManager::Handler(char* a, char* b)
{
    *a = 1;
    *b = (char)(*a + 1);
}

CTexture& CDisplayManager::CopyBackBufferToTexture()
{
    char* p;
    char* q;
    { char b; q = &b; }
    { char a; p = &a; }
    this->Handler(q, p);
    return *reinterpret_cast<CTexture*>(this);
}

int main()
{
    CDisplayManager dm;
    dm.tag = 42;
    CTexture& r = dm.CopyBackBufferToTexture();
    if (reinterpret_cast<CDisplayManager*>(&r) == &dm)
        printf("PARITY_OK\n");
    return 0;
}