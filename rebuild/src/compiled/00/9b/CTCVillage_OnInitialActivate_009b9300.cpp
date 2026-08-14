// Byte-exact reconstruction of CTCVillage::OnInitialActivate @ 0x009b9300

struct CTCVillage {
    void* field0;   // +0x00
    void* field4;   // +0x04
    char  pad8[4];  // +0x08
    unsigned char activateArg;   // +0x0c
    unsigned char pendingFlag;   // +0x0d
};

extern "C" void __fastcall Callee_9b8ef0(void* self, void* edxarg, unsigned char arg);

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->pendingFlag) {
        Callee_9b8ef0(self->field0, self->field4, self->activateArg);
        self->pendingFlag = 0;
    }
}