#include <cstdio>

struct Inner {
    bool Build(void* a, long b, int zero);
};

struct CWideString {
    char pad0[8];
    Inner* field8;
};

static void* g_a; static long g_b; static int g_zero; static Inner* g_this;

bool Inner::Build(void* a, long b, int zero) {
    g_this = this; g_a = a; g_b = b; g_zero = zero;
    return true;
}

bool __fastcall ConstructString(CWideString* self, int /*edx*/, void* a, long b)
{
    return self->field8->Build(a, b, 0);
}

int main() {
    Inner inner;
    CWideString ws;
    ws.field8 = &inner;
    void* pa = (void*)0x1234;
    long bb = 0x5678;
    bool r = ConstructString(&ws, 0, pa, bb);
    if (r && g_this == &inner && g_a == pa && g_b == bb && g_zero == 0)
        printf("00a35b50_TEST PASS\n");
    else
        printf("FAIL r=%d\n", (int)r);
    return 0;
}