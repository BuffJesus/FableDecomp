// Byte-exact reconstruction of CTCShotDefinitionBase::IsToDie @ 0x00923cb0
struct CTCShotDefinitionBase {
    char pad[0x34];
    int m_dieTime; // +0x34
};

extern int __cdecl GetCurrentValue_49d870(void);

int __fastcall IsToDie(CTCShotDefinitionBase* self)
{
    int t = self->m_dieTime;
    if (t != 0) {
        if (GetCurrentValue_49d870() > self->m_dieTime)
            return 1;
    }
    return 0;
}