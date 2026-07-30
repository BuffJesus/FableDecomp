extern unsigned char g_aggressiveEnabled;

struct CPlayer;
extern "C" void __fastcall CPlayer_OnAggressiveChanged(CPlayer* self);

struct CPlayer {
    char pad[0x217];
    unsigned char aggressive;
};

void __fastcall ToggleAggressiveMode(CPlayer* self)
{
    if (g_aggressiveEnabled) {
        self->aggressive = (self->aggressive == 0);
        CPlayer_OnAggressiveChanged(self);
    }
}