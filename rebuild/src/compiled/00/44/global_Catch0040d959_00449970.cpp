struct CPlayer;
struct CPlayerManager449970 { unsigned char unused[0x1c]; int index; CPlayer* GetPlayer(int value); };
void __fastcall Catch_0040d959(CPlayerManager449970* self) { self->GetPlayer(self->index); }