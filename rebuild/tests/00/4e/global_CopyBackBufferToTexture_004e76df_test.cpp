#include <cstdio>

struct CTexture;

struct CDisplayManager {
    int calls;
    char* seen_a;
    char* seen_b;
    CTexture* CopyBackBufferToTexture();
    void Helper(char* a, char* b);
};

void CDisplayManager::Helper(char* a, char* b)
{
    this->calls++;
    this->seen_a = a;
    this->seen_b = b;
}

CTexture* CDisplayManager::CopyBackBufferToTexture()
{
    char* p1;
    char* p2;
    { char scratch; p1 = &scratch; }
    { char scratch; p2 = &scratch; }
    this->Helper(p1, p2);
    return (CTexture*)this;
}

int main()
{
    CDisplayManager d;
    d.calls = 0;
    d.seen_a = 0;
    d.seen_b = 0;
    CTexture* r = d.CopyBackBufferToTexture();
    bool ok = (r == (CTexture*)&d) && (d.calls == 1) && (d.seen_a != 0) && (d.seen_b != 0);
    printf("%s\n", ok ? "PASS_004e76df" : "FAIL_004e76df");
    return 0;
}