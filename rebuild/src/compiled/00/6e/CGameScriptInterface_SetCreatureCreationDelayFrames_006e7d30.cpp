struct CGameScriptInterface {
    unsigned char _pad0[0x4c];
    int creatureCreationDelayFrames; // +0x4c
    void SetCreatureCreationDelayFrames(int frames);
};

void CGameScriptInterface::SetCreatureCreationDelayFrames(int frames)
{
    this->creatureCreationDelayFrames = frames;
}