struct CPlayer;
struct CPlayerManager449980 { unsigned char unused[0x1c]; int index; CPlayer* GetPlayer(int value); };
void __fastcall DrawGetEnvironment(CPlayer* player);
void __fastcall Catch_0042ae0a(CPlayerManager449980* self) { DrawGetEnvironment(self->GetPlayer(self->index)); }