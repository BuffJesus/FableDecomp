#include "engine/CFileDataOutputStream.h"
#include <cstdio>

struct CAFile {
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void v4(const void* p, long n, int zero) = 0;
};

void __fastcall Write(CFileDataOutputStream* self, int, const void* p, long n);

static CAFile* g_self;
static long   g_n;
static const void* g_p;
static int    g_zero = 99;

struct Sink : CAFile {
    void v0() {}
    void v1() {}
    void v2() {}
    void v3() {}
    void v4(const void* p, long n, int zero) {
        g_self = this; g_n = n; g_p = p; g_zero = zero;
    }
};

int main()
{
    Sink sink;
    CFileDataOutputStream s;
    s.File = &sink;

    char buf[4];
    Write(&s, 0, &buf[0], 42);

    if (g_self == &sink && g_n == 42 && g_p == &buf[0] && g_zero == 0)
        printf("00994480_TEST PASS\n");
    else
        printf("FAIL self=%p n=%ld p=%p zero=%d\n", (void*)g_self, g_n, g_p, g_zero);
    return 0;
}