#include <cstdio>

struct CCompressedMemoryDataInputStream {
    void* vtbl;
    char pad[0xa4];
    void* buf_a8;
    char pad2[0x8];
    void* buf_b4;
};

static int g_freed = 0;
static void* g_last_free = 0;
static int g_base_called = 0;

void __cdecl operator delete(void* p) { g_freed++; g_last_free = p; }
void __fastcall CBase_dtor(void* self) { g_base_called++; }

void __fastcall CCompressedMemoryDataInputStream_dtor(CCompressedMemoryDataInputStream* self)
{
    if (self->buf_b4)
        operator delete(self->buf_b4);
    if (self->buf_a8)
        operator delete(self->buf_a8);
    self->vtbl = (void*)0x1230ba0;
    CBase_dtor(self);
}

int main()
{
    CCompressedMemoryDataInputStream o;
    o.vtbl = 0;
    o.buf_a8 = (void*)0x1111;
    o.buf_b4 = (void*)0x2222;
    CCompressedMemoryDataInputStream_dtor(&o);
    if (g_freed == 2 && o.vtbl == (void*)0x1230ba0 && g_base_called == 1) {
        // also test null path
        g_freed = 0; g_base_called = 0;
        CCompressedMemoryDataInputStream o2;
        o2.vtbl = 0; o2.buf_a8 = 0; o2.buf_b4 = 0;
        CCompressedMemoryDataInputStream_dtor(&o2);
        if (g_freed == 0 && g_base_called == 1 && o2.vtbl == (void*)0x1230ba0) {
            std::printf("CCompressedMemoryDataInputStream_00453b15_TEST PASS\n");
            return 0;
        }
    }
    std::printf("FAIL\n");
    return 1;
}