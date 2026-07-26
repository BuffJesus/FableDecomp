#include <stdio.h>

#include "fable_profile.h"

int main()
{
    NProfileTimer::EndProfile(0);
    NProfileTimer::EndProfile(17);
    NProfileTimer::EndProfile(-1);

    printf("FABLETLC_PROFILE_END_BEHAVIOR PASS\n");
    return 0;
}
