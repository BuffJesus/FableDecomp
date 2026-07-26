#include <stdio.h>

#include "fable_text_layout.h"

bool g_FablePrimaryLeftAlignment_013CA7EA = false;

int main()
{
    NTextLayoutConfig::SetPrimaryLeftAlignment(true);
    if (!g_FablePrimaryLeftAlignment_013CA7EA)
    {
        printf("FABLETLC_PRIMARY_LEFT_ALIGNMENT_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    NTextLayoutConfig::SetPrimaryLeftAlignment(false);
    if (g_FablePrimaryLeftAlignment_013CA7EA)
    {
        printf("FABLETLC_PRIMARY_LEFT_ALIGNMENT_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    printf("FABLETLC_PRIMARY_LEFT_ALIGNMENT_BEHAVIOR PASS\n");
    return 0;
}
