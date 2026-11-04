#include "fable_game.h"
#include <stdio.h>
fable_u8 g_FableCompileFrontendDefinitions_013B8648;
static bool calls[3];
static unsigned count;
void FABLE_FASTCALL CNewFrontendGameComponent::CompileDefsPlatformSpecific(bool xbox) {
    if (count < 3) calls[count] = xbox;
    ++count;
}
int main() {
    g_FableCompileFrontendDefinitions_013B8648 = 0;
    CNewFrontendGameComponent::CompileDefs();
    if (count != 1 || calls[0]) return 1;
    count = 0;
    g_FableCompileFrontendDefinitions_013B8648 = 1;
    CNewFrontendGameComponent::CompileDefs();
    if (count != 2 || !calls[0] || calls[1]) return 2;
    puts("PASS_0042f70b");
    return 0;
}
