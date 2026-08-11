// CModeChargeForcePush::OnActivate  @ 008d6900
// __fastcall void OnActivate(CModeChargeForcePush* this)

struct CMember {
    char pad[0x10];
    unsigned char flag;                    // +0x10
    void DoPush(int a, int b);             // __fastcall, out-of-line @ 0x833a10
};

struct CModeChargeForcePush {
    void*    base;        // +0x00
    CMember* member;      // +0x04
};

// external target
extern void __fastcall CMode_Base_OnActivate(void* self);      // 0x861d00

void __fastcall CModeChargeForcePush_OnActivate(CModeChargeForcePush* self)
{
    CMember* m;
    CMode_Base_OnActivate(self);
    m = self->member;
    if (m->flag == 1)
        m->DoPush(0, 0);
}