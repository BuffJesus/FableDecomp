#include <stdio.h>

#include "fable_text_layout.h"

bool g_FableSecondaryLeftAlignment_013CA7EB = false;

int main()
{
    NTextLayoutConfig::SetSecondaryLeftAlignment(true);
    if (!g_FableSecondaryLeftAlignment_013CA7EB)
    {
        printf("FABLETLC_SECONDARY_LEFT_ALIGNMENT_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    NTextLayoutConfig::SetSecondaryLeftAlignment(false);
    if (g_FableSecondaryLeftAlignment_013CA7EB)
    {
        printf("FABLETLC_SECONDARY_LEFT_ALIGNMENT_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    printf("FABLETLC_SECONDARY_LEFT_ALIGNMENT_BEHAVIOR PASS\n");
    return 0;
}
