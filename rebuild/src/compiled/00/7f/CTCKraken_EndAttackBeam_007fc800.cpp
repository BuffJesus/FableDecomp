#pragma optimize("s",on)

struct CTCKraken {
    char pad[0x64];
    int  beamState;
};

extern "C" void __fastcall Kraken_PreEnd(CTCKraken* self);
extern "C" void __fastcall Kraken_Finish(CTCKraken* self);

void __fastcall EndAttackBeam(CTCKraken* self)
{
    self->beamState = 3;
    Kraken_PreEnd(self);
    Kraken_Finish(self);
}