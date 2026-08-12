#include <cstdio>
#include <cstdlib>

static int g_freed = 0;
static void* g_lastFreed = (void*)0;

void FreePackedUIntArrayBuffer(void* p)
{
    g_freed++;
    g_lastFreed = p;
}

struct CPackedUIntArray
{
    void* buffer;
    int   f4;
    char  f8;
    char  pad9;
    char  padA;
    char  padB;
    int   fC;
    ~CPackedUIntArray();
};

CPackedUIntArray::~CPackedUIntArray()
{
    FreePackedUIntArrayBuffer(this->buffer);
    this->buffer = 0;
    this->f4 = 0;
    this->f8 = 0;
    this->fC = 0;
}

int main()
{
    CPackedUIntArray a;
    void* marker = (void*)0xDEADBEEF;
    a.buffer = marker;
    a.f4 = 123;
    a.f8 = 7;
    a.fC = 456;

    a.~CPackedUIntArray();

    int ok = (g_freed == 1) && (g_lastFreed == marker) &&
             (a.buffer == 0) && (a.f4 == 0) && (a.f8 == 0) && (a.fC == 0);

    if (ok) printf("DTOR_OK\n");
    else    printf("DTOR_FAIL\n");
    return ok ? 0 : 1;
}