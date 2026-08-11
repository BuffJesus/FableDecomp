// CTCInventoryQuests::ProcessButtonBReleased @ 0061b5f0
// void __fastcall (modelled __fastcall self in ecx)

struct CTCInventoryQuests {
    char pad[0x157];
    unsigned char m_flag;   // +0x157
};

// helper A @ 0x61ac60 (non-virtual member call on this)
void __fastcall CTCInventoryQuests_HelperA(CTCInventoryQuests* self);
// helper B @ 0x5bc66f (tail-called member on this)
void __fastcall CTCInventoryQuests_HelperB(CTCInventoryQuests* self);

void __fastcall CTCInventoryQuests_ProcessButtonBReleased(CTCInventoryQuests* self)
{
    if (self->m_flag)
        CTCInventoryQuests_HelperA(self);
    CTCInventoryQuests_HelperB(self);
}