#include <cstdio>

struct CGameScriptInterface {
    unsigned char _pad0[0x4c];
    int creatureCreationDelayFrames; // +0x4c
    void SetCreatureCreationDelayFrames(int frames);
};

void CGameScriptInterface::SetCreatureCreationDelayFrames(int frames)
{
    this->creatureCreationDelayFrames = frames;
}

int main()
{
    CGameScriptInterface obj;
    obj.creatureCreationDelayFrames = 0;
    obj.SetCreatureCreationDelayFrames(42);
    if (obj.creatureCreationDelayFrames != 42) { std::printf("FAIL: got %d\n", obj.creatureCreationDelayFrames); return 1; }
    obj.SetCreatureCreationDelayFrames(-7);
    if (obj.creatureCreationDelayFrames != -7) { std::printf("FAIL: got %d\n", obj.creatureCreationDelayFrames); return 1; }
    std::printf("CGameScriptInterface_006e7d30_TEST PASS\n");
    return 0;
}