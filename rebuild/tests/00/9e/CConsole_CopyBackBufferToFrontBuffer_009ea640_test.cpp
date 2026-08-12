#include <cstdio>

struct CConBuf {
    void* vp;
    unsigned char b;
    int d;
    void Assign(CConBuf* src);
};

struct CConsole {
    char pad20[0x20];
    CConBuf front;
    char pad2c[4];
    CConBuf back;
    void CopyBackBufferToFrontBuffer();
};

// stand-in for 0x99efb0
void CConBuf::Assign(CConBuf* src) { this->vp = src->vp; }

void CConsole::CopyBackBufferToFrontBuffer()
{
    this->front.Assign(&this->back);
    this->front.b = this->back.b;
    this->front.d = this->back.d;
}

int main()
{
    CConsole c;
    for (int i = 0; i < (int)sizeof(c); i++) ((char*)&c)[i] = 0;
    c.back.vp = (void*)0x12345678;
    c.back.b = 0xAB;
    c.back.d = 0x0BADF00D;
    c.front.vp = 0;
    c.front.b = 0;
    c.front.d = 0;
    c.CopyBackBufferToFrontBuffer();
    bool ok = (c.front.vp == (void*)0x12345678) &&
              (c.front.b == 0xAB) &&
              (c.front.d == 0x0BADF00D);
    printf("front.b=%02X front.d=%08X\n", c.front.b, (unsigned)c.front.d);
    if (ok) { printf("COPYBB_OK\n"); return 0; }
    printf("COPYBB_FAIL\n");
    return 1;
}