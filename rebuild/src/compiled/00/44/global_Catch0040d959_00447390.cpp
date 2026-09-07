struct CGamePlayerInterface {
    unsigned char unused[0x7a4]; int control_scheme;
    void SetControlScheme(int value);
};
void __fastcall Catch_0040d959(CGamePlayerInterface* self) { self->SetControlScheme(self->control_scheme); }