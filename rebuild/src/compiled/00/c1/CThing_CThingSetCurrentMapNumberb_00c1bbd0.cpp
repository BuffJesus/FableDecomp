struct CThing { char pad[4]; short currentMapNumber_b; };
void __fastcall SetCurrentMapNumber_b(CThing *self, int /*edx*/, short mapNumber) {
    self->currentMapNumber_b = mapNumber;
}