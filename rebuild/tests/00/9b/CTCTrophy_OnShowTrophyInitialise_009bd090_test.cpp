#include <stdio.h>
static int g_prepares;
struct CTCTrophy { char pad[0x54]; int showInitialiseCount; void PrepareTrophy(); void OnShowTrophyInitialise(); };
void CTCTrophy::PrepareTrophy() { ++g_prepares; }
void CTCTrophy::OnShowTrophyInitialise() { PrepareTrophy(); ++showInitialiseCount; }
int main() {
    CTCTrophy trophy; trophy.showInitialiseCount = 4; g_prepares = 0;
    trophy.OnShowTrophyInitialise();
    if (trophy.showInitialiseCount != 5 || g_prepares != 1) return 1;
    printf("TROPHY_INITIALISE PASS\n"); return 0;
}