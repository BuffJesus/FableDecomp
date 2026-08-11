#include <cstdio>

struct CVertexBufferWin32 {
    char pad[0x3a3c];
    unsigned long baseSize;
    unsigned long DoSizeof();
};

unsigned long CVertexBufferWin32::DoSizeof()
{
    return this->baseSize + 0x204;
}

int main()
{
    CVertexBufferWin32 vb;
    vb.baseSize = 1000;
    unsigned long r = vb.DoSizeof();
    if (r != 1000UL + 0x204UL) { printf("FAIL %lu\n", r); return 1; }
    vb.baseSize = 0;
    if (vb.DoSizeof() != 0x204UL) { printf("FAIL zero\n"); return 1; }
    printf("DOSIZEOF_OK\n");
    return 0;
}