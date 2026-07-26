#include <stdio.h>

#include "rebuild_abi.h"

typedef fable_u32 (FABLE_FASTCALL *FableWordWidthCallback)(fable_u16 value);
typedef fable_u32 (FABLE_FASTCALL *FableLineHeightCallback)();

FableWordWidthCallback g_FableWordWidthCallback_013CE758 = 0;
FableLineHeightCallback g_FableLineHeightCallback_013CE75C = 0;

void FABLE_FASTCALL WordWrap_SetCallback(
    FableWordWidthCallback wordWidth,
    FableLineHeightCallback lineHeight);

static fable_u32 FABLE_FASTCALL WordWidthA(fable_u16)
{
    return 10;
}

static fable_u32 FABLE_FASTCALL WordWidthB(fable_u16)
{
    return 20;
}

static fable_u32 FABLE_FASTCALL LineHeightA()
{
    return 30;
}

int main()
{
    WordWrap_SetCallback(&WordWidthA, &LineHeightA);
    if (
        g_FableWordWidthCallback_013CE758 != &WordWidthA ||
        g_FableLineHeightCallback_013CE75C != &LineHeightA)
    {
        return 1;
    }

    WordWrap_SetCallback(&WordWidthB, 0);
    if (
        g_FableWordWidthCallback_013CE758 != &WordWidthB ||
        g_FableLineHeightCallback_013CE75C != &LineHeightA)
    {
        return 1;
    }

    WordWrap_SetCallback(0, 0);
    if (
        g_FableWordWidthCallback_013CE758 != &WordWidthB ||
        g_FableLineHeightCallback_013CE75C != &LineHeightA)
    {
        return 1;
    }

    printf("FABLETLC_WORD_WRAP_CALLBACK_BEHAVIOR PASS\n");
    return 0;
}
