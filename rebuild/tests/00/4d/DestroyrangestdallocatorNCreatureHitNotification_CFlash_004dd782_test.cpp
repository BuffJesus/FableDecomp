#include <stdio.h>

struct CFlash
{
    unsigned long a;
    void* p;
    void DestroyElement(int mode);
};

struct allocator_flash {};

static int g_calls = 0;
static void* g_last = 0;
static int g_lastmode = -1;

// Out-of-line element destructor stand-in (retail 0x4da68a). __fastcall member.
void CFlash::DestroyElement(int mode)
{
    ++g_calls;
    g_last = this;
    g_lastmode = mode;
}

void __fastcall Destroy_range_CFlash(CFlash* first, CFlash* last, allocator_flash* al);

int main()
{
    CFlash buf[4];
    allocator_flash al;

    g_calls = 0;
    Destroy_range_CFlash(&buf[0], &buf[3], &al);   // 3 elements, stride 8
    if (g_calls != 3) { printf("FAIL count=%d\n", g_calls); return 1; }
    if (g_last != &buf[2]) { printf("FAIL last\n"); return 1; }
    if (g_lastmode != 0) { printf("FAIL mode=%d\n", g_lastmode); return 1; }

    // empty range => no calls
    g_calls = 0;
    Destroy_range_CFlash(&buf[0], &buf[0], &al);
    if (g_calls != 0) { printf("FAIL empty=%d\n", g_calls); return 1; }

    printf("DESTROY_RANGE_FLASH_OK\n");
    return 0;
}