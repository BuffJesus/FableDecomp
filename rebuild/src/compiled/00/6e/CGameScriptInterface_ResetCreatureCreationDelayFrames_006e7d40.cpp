struct CGameScriptInterface {
    char pad[0x4c];
    int creatureCreationDelayFrames;
};

void __fastcall CGameScriptInterface_ResetCreatureCreationDelayFrames(CGameScriptInterface* self)
{
    self->creatureCreationDelayFrames = 0;
}