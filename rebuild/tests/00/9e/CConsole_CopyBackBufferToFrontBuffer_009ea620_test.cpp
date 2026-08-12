#include <cstdio>

struct ConBuffer {
    void*         data;
    unsigned char flag;
    int           extra;
    int           pad;
    void CopyContents(ConBuffer* src);
};

struct CConsole {
    char      pad0[0x20];
    ConBuffer back;
    ConBuffer front;
    void CopyBackBufferToFrontBuffer();
};

static int g_copied = 0;
void ConBuffer::CopyContents(ConBuffer* src)
{
    this->data = src->data;
    g_copied = 1;
}

void CConsole::CopyBackBufferToFrontBuffer()
{
    this->front.CopyContents(&this->back);
    this->front.flag = this->back.flag;
    int* pd = &this->front.extra;
    int* ps = &this->back.extra;
    *pd = *ps;
}

int main()
{
    CConsole c;
    c.back.data  = (void*)0x1234;
    c.back.flag  = 0x7A;
    c.back.extra = 0x55667788;
    c.front.data = 0;
    c.front.flag = 0;
    c.front.extra = 0;

    c.CopyBackBufferToFrontBuffer();

    bool ok = g_copied == 1
        && c.front.data  == (void*)0x1234
        && c.front.flag  == 0x7A
        && c.front.extra == 0x55667788;

    if (ok) printf("COPYBACK_OK\n");
    else    printf("COPYBACK_FAIL\n");
    return ok ? 0 : 1;
}