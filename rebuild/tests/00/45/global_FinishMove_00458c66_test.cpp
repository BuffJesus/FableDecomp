#include "rebuild_abi.h"
#include <stdio.h>

struct CTCAssassinRush {
    char _pad[0x68];
};

extern "C" void __fastcall AssassinRush_Sub0x68_Method(void* thisptr);
extern "C" void __cdecl AssassinRush_Free(void* p);
extern "C" void __fastcall AssassinRush_BaseFinishMove(void* thisptr);
void __fastcall CTCAssassinRush_FinishMove(CTCAssassinRush* thisptr);

static int g_sub = 0;
static int g_free = 0;
static int g_base = 0;
static void* g_freed = 0;

extern "C" void __fastcall AssassinRush_Sub0x68_Method(void* thisptr) { g_sub++; }
extern "C" void __cdecl AssassinRush_Free(void* p) { g_free++; g_freed = p; }
extern "C" void __fastcall AssassinRush_BaseFinishMove(void* thisptr) { g_base++; }

int main()
{
    unsigned char buf[0x70];
    for (int i = 0; i < 0x70; ++i) buf[i] = 0;
    CTCAssassinRush* obj = (CTCAssassinRush*)buf;

    // non-null pointer at +0x50 -> should free
    void* marker = (void*)0x12345678;
    *(void**)(buf + 0x50) = marker;

    CTCAssassinRush_FinishMove(obj);

    int ok1 = (g_sub == 1 && g_free == 1 && g_base == 1 && g_freed == marker);

    // null pointer at +0x50 -> should NOT free
    g_sub = g_free = g_base = 0; g_freed = 0;
    *(void**)(buf + 0x50) = 0;
    CTCAssassinRush_FinishMove(obj);
    int ok2 = (g_sub == 1 && g_free == 0 && g_base == 1);

    if (ok1 && ok2)
        printf("PASS_00458c66\n");
    else
        printf("FAIL_00458c66 %d %d\n", ok1, ok2);
    return 0;
}