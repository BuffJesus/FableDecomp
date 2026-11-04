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
int main() {
    memset(storage,0xA5,sizeof(storage)); memcpy(before,storage,sizeof(storage));
    TimingAccess* game=reinterpret_cast<TimingAccess*>(storage);
    CFrameRateSmoother* smoother=reinterpret_cast<CFrameRateSmoother*>(storage);
    *reinterpret_cast<double*>(storage+0x28)=0.03125;
    memcpy(before,storage,sizeof(storage));
    if (smoother->GetSmoothedTime()!=0.03125) return 1;
    if (memcmp(before,storage,sizeof(storage))) return 3;
    puts("AUTO_TINY_0049b990_TEST PASS"); return 0;
}
