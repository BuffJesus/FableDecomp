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
static const void* receiver; static double argument;
void CFrameRateSmoother::AddFrameTime(double value) { ++calls; receiver=this; argument=value; }
int main() {
    memset(storage,0xA5,sizeof(storage)); memcpy(before,storage,sizeof(storage));
    TimingAccess* game=reinterpret_cast<TimingAccess*>(storage);
    game->UpdateAverageFrameDuration(-0.125);
    if (calls!=1 || receiver!=storage+0x16178 || argument!=-0.125) return 1;
    if (memcmp(before,storage,sizeof(storage))) return 3;
    puts("PASS_00416202"); return 0;
}
