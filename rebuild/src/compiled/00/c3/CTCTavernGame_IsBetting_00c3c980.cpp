struct CTCTavernGame {
    char pad0[0xc];
    int  field_c;   // +0x0c, signed
    int  field_10;  // +0x10
};

bool __fastcall IsBetting(CTCTavernGame *self)
{
    return self->field_10 != 0 || self->field_c >= 0;
}