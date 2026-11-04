#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_font_state_blocks.h"

void* __fastcall CStreamingFontBankStateBlock_DeletingDestructor(CStreamingFontBankStateBlock*, void*, unsigned);
static void* g_expectedFree;
static unsigned g_frees;
void operator delete(void* memory)
{
    if (memory == g_expectedFree)
        ++g_frees;
    free(memory);
}
int main()
{
    unsigned char storage[sizeof(CStreamingFontBankStateBlock) + 4];
    memset(storage, 0xA5, sizeof(storage));
    CStreamingFontBankStateBlock* block = reinterpret_cast<CStreamingFontBankStateBlock*>(storage);
    if (CStreamingFontBankStateBlock_DeletingDestructor(block, 0, 0) != block || g_frees)
        return 1;
    if (*reinterpret_cast<unsigned long*>(storage) == 0xA5A5A5A5)
        return 2;
    for (unsigned i = 4; i < sizeof(storage); ++i)
        if (storage[i] != 0xA5) return 3;
    if (CStreamingFontBankStateBlock_DeletingDestructor(block, 0, 2) != block || g_frees)
        return 4;
    g_expectedFree = malloc(sizeof(CStreamingFontBankStateBlock));
    memset(g_expectedFree, 0xA5, sizeof(CStreamingFontBankStateBlock));
    void* returned = CStreamingFontBankStateBlock_DeletingDestructor(
        static_cast<CStreamingFontBankStateBlock*>(g_expectedFree), 0, 1);
    if (returned != g_expectedFree || g_frees != 1)
        return 5;
    puts("STATE_DELETE_00a5ee70_PASS");
    return 0;
}
