// CTCHeroAttachableAppearanceModifiers::HandleMessage @ 00705fe0

struct Msg;

struct MsgVtbl {
    int (__fastcall *slot0)(Msg* self);
    int (__fastcall *slot1)(Msg* self);
};

struct Msg {
    MsgVtbl* vtbl;
};

struct CTCHeroAttachableAppearanceModifiers {
    char pad[0x14];
    unsigned char f14;
};

void __fastcall HandleMessage(CTCHeroAttachableAppearanceModifiers* self, int /*edx*/, Msg* msg)
{
    if (msg->vtbl->slot1(msg) == 3)
        self->f14 |= 1;
}