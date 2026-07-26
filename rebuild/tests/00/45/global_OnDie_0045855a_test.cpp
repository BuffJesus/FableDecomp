#include "rebuild_abi.h"
#include <cstdio>

struct CTCInventoryItem
{
    char pad[0x3c];
    void* m_ptr;
};

extern void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* thisptr);

static int g_freed = 0;
static int g_based = 0;

extern "C" void __cdecl engine_free_0045855a(void* p) { (void)p; g_freed = 1; }
void __fastcall base_OnDie_0045855a(CTCInventoryItem* t) { (void)t; g_based = 1; }

int main()
{
    CTCInventoryItem a;
    for (int i = 0; i < (int)sizeof(a); ++i) ((char*)&a)[i] = 0;
    a.m_ptr = (void*)0x1234;
    CTCInventoryItem_OnDie(&a);
    if (g_freed == 1 && g_based == 1)
        printf("OK_0045855a_FREED_BASED\n");
    return 0;
}