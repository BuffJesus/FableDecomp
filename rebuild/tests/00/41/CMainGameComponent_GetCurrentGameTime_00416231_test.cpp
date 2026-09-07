#include <stdio.h>
struct CMainGameComponent {
    char pad[0x60];
    double gameTimeBase;
    double GetCurrentGameTime();
};
static double g_now;
double __fastcall ReadCurrentGameTime(CMainGameComponent*) { return g_now; }
double CMainGameComponent::GetCurrentGameTime() { return ReadCurrentGameTime(this) - gameTimeBase; }
int main() {
    CMainGameComponent game;
    g_now = 17.5; game.gameTimeBase = 6.25;
    if (game.GetCurrentGameTime() != 11.25) return 1;
    printf("CURRENT_GAME_TIME PASS\n");
    return 0;
}