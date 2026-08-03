#include <stdio.h>

struct CVertexBufferWin32 { char pad[0x60]; int baseSize; };
int __fastcall DoSizeof(CVertexBufferWin32* self)
{
    return self->baseSize + 0xc;
}

int main()
{
    CVertexBufferWin32 obj;
    obj.baseSize = 100;
    int r = DoSizeof(&obj);
    if (r != 112) { printf("FAIL got %d\n", r); return 1; }
    obj.baseSize = 0;
    if (DoSizeof(&obj) != 12) { printf("FAIL zero\n"); return 1; }
    printf("OK_0x004c73d0 sizeof=%d\n", r);
    return 0;
}