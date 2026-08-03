struct CThing { char pad[0x8e]; short currentMapNumber; };
void __fastcall SetCurrentMapNumber(CThing *self, int /*edx*/, short mapNumber) {
    self->currentMapNumber = mapNumber;
}