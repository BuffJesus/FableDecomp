#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_font_state_blocks.h"

void* __fastcall CFontBankStateBlock_DeletingDestructor(CFontBankStateBlock*, void*, unsigned);
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
    unsigned char storage[sizeof(CFontBankStateBlock) + 4];
    memset(storage, 0xA5, sizeof(storage));
    CFontBankStateBlock* block = reinterpret_cast<CFontBankStateBlock*>(storage);
    if (CFontBankStateBlock_DeletingDestructor(block, 0, 0) != block || g_frees)
        return 1;
    if (*reinterpret_cast<unsigned long*>(storage) == 0xA5A5A5A5)
        return 2;
    for (unsigned i = 4; i < sizeof(storage); ++i)
        if (storage[i] != 0xA5) return 3;
    if (CFontBankStateBlock_DeletingDestructor(block, 0, 2) != block || g_frees)
        return 4;
    g_expectedFree = malloc(sizeof(CFontBankStateBlock));
    memset(g_expectedFree, 0xA5, sizeof(CFontBankStateBlock));
    void* returned = CFontBankStateBlock_DeletingDestructor(
        static_cast<CFontBankStateBlock*>(g_expectedFree), 0, 1);
    if (returned != g_expectedFree || g_frees != 1)
        return 5;
    puts("STATE_DELETE_00a63320_PASS");
    return 0;
}
