#include <cstdio>

struct CGameScriptInterface {
    char pad[0x4c];
    int creatureCreationDelayFrames;
};

void __fastcall CGameScriptInterface_ResetCreatureCreationDelayFrames(CGameScriptInterface* self)
{
    self->creatureCreationDelayFrames = 0;
}

int main()
{
    CGameScriptInterface obj;
    obj.creatureCreationDelayFrames = 12345;
    CGameScriptInterface_ResetCreatureCreationDelayFrames(&obj);
    if (obj.creatureCreationDelayFrames != 0) {
        std::printf("FAIL: expected 0 got %d\n", obj.creatureCreationDelayFrames);
        return 1;
    }
    std::printf("CGameScriptInterface_006e7d40_TEST PASS\n");
    return 0;
}