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
static const void* receiver;
double CMainGameComponent::GetCurrentGameTime() const { ++calls; receiver=this; return 7.0; }
int main() {
    memset(storage,0xA5,sizeof(storage)); memcpy(before,storage,sizeof(storage));
    TimingAccess* game=reinterpret_cast<TimingAccess*>(storage);
    *reinterpret_cast<double*>(storage+0x161A8)=6.75;
    memcpy(before,storage,sizeof(storage));
    if (game->GetPredictedTimeSinceLastRenderFrame()!=0.25 || calls!=1 || receiver!=game) return 1;
    if (memcmp(before,storage,sizeof(storage))) return 3;
    puts("00416258_TEST PASS"); return 0;
}
