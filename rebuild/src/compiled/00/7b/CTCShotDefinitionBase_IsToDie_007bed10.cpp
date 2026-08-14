struct CTCShotDefinitionBase {
    char pad[0x60];
    int TimeToDie;   // +0x60
};

extern "C" int __cdecl GetCurrentGameTime(void);

int __fastcall IsToDie(CTCShotDefinitionBase *self)
{
    int t = self->TimeToDie;
    if (t > 0) {
        if (GetCurrentGameTime() > self->TimeToDie)
            return 1;
    }
    return 0;
}