#include <stdio.h>

struct CTCKraken {
    char pad[0x64];
    int  beamState;
};

static int g_pre = 0;
static int g_fin = 0;
static CTCKraken* g_last = 0;

extern "C" void __fastcall Kraken_PreEnd(CTCKraken* self){ g_pre++; g_last = self; }
extern "C" void __fastcall Kraken_Finish(CTCKraken* self){ g_fin++; g_last = self; }

void __fastcall EndAttackBeam(CTCKraken* self);

int main(){
    CTCKraken k;
    k.beamState = 0;
    EndAttackBeam(&k);
    if (k.beamState == 3 && g_pre == 1 && g_fin == 1 && g_last == &k)
        printf("OK_007fc800\n");
    else
        printf("FAIL %d %d %d\n", k.beamState, g_pre, g_fin);
    return 0;
}