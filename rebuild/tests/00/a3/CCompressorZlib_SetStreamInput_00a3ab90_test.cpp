#include <stdio.h>
#include <string.h>

struct CCompressorZlib {
    char pad[0x1c];
    void* streamInput;   // +0x1c
    unsigned long size;  // +0x20
    int field24;         // +0x24
    int field28;         // +0x28
    int field2c;         // +0x2c
    void SetStreamInput(void* input, unsigned long size);
};

void CCompressorZlib::SetStreamInput(void* input, unsigned long sz)
{
    this->streamInput = input;
    this->field28 = 0;
    this->size = sz;
    this->field2c = 0;
}

int main()
{
    CCompressorZlib c;
    memset(&c, 0xAB, sizeof(c));
    char buf[16];
    c.SetStreamInput(buf, 12345u);

    int ok = 1;
    if (c.streamInput != (void*)buf) ok = 0;
    if (c.size != 12345u) ok = 0;
    if (c.field28 != 0) ok = 0;
    if (c.field2c != 0) ok = 0;
    if (c.field24 != (int)0xABABABAB) ok = 0; // untouched

    if (ok) printf("SETSTREAMINPUT_OK\n");
    else printf("FAIL\n");
    return ok ? 0 : 1;
}