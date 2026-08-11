// SetAsPickUpNewLockTarget @ 008023a0
struct CTCTargetingPlayer {
    char pad[0x3c];
    unsigned char f3c;   // +0x3c
    char pad2[3];        // align to +0x40
    unsigned int f40;    // +0x40
    unsigned int f44;    // +0x44
};

extern "C" unsigned int __fastcall helper_49d870(CTCTargetingPlayer* self);

void __fastcall SetAsPickUpNewLockTarget(CTCTargetingPlayer* self)
{
    self->f3c = 1;
    self->f40 = 0;
    self->f44 = helper_49d870(self);
}