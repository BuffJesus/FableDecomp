struct CTCCoopSpirit_0066fb00
{
    unsigned char prefix[0x10];
    long experience;
};

extern "C" void __fastcall
CTCCoopSpirit_AddExperience_0066fb00(
    CTCCoopSpirit_0066fb00* self, void*, long amount)
{
    self->experience += amount;
}
