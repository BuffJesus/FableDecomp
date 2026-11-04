#include "fable_game.h"
#include <stdio.h>
#include <string.h>
class TimingAccess : public CMainGameComponent {
public:
    using CMainGameComponent::UpdateAverageFrameDuration;
    using CMainGameComponent::GetCurrentGameTime;
    using CMainGameComponent::GetPredictedTimeSinceLastRenderFrame;
};
static unsigned char storage[sizeof(CMainGameComponent) + 8];
static unsigned char before[sizeof(storage)];
static int calls;
double FABLE_FASTCALL GFGetTime() { ++calls; return 17.5; }
int main() {
    memset(storage,0xA5,sizeof(storage)); memcpy(before,storage,sizeof(storage));
    TimingAccess* game=reinterpret_cast<TimingAccess*>(storage);
    *reinterpret_cast<double*>(storage+0x60)=6.25;
    memcpy(before,storage,sizeof(storage));
    if (game->GetCurrentGameTime()!=11.25 || calls!=1) return 1;
    *reinterpret_cast<double*>(storage+0x60)=20.0;
    memcpy(before,storage,sizeof(storage));
    if (game->GetCurrentGameTime()!=-2.5 || calls!=2) return 2;
    if (memcmp(before,storage,sizeof(storage))) return 3;
    puts("CURRENT_GAME_TIME PASS"); return 0;
}
