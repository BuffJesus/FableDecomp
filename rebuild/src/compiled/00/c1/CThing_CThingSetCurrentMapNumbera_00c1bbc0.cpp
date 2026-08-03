struct CThing {
    char pad[10];
    unsigned short mapNumber;

    void SetCurrentMapNumber_a(unsigned short newMapNumber)
    {
        mapNumber = newMapNumber;
    }
};

void __fastcall CThing_SetCurrentMapNumber_a(CThing* self, void* /*edx*/, unsigned short newMapNumber)
{
    self->SetCurrentMapNumber_a(newMapNumber);
}