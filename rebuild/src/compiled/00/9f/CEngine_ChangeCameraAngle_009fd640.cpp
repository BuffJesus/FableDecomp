// ChangeCameraAngle @ 009fd640  (CEngine::ChangeCameraAngle)
struct CCameraSub;

// The method called on the sub-object at [this+0x280]; __fastcall, no args, void.
void __fastcall CCameraSub_Reset(CCameraSub* self);

struct CEngine {
    char pad0[0x1f8];
    unsigned char flag_1f8;   // [this+0x1f8]
    char pad1[0x280 - 0x1f8 - 1];
    CCameraSub* sub_280;      // [this+0x280]
};

void __fastcall ChangeCameraAngle(CEngine* self)
{
    CCameraSub_Reset(self->sub_280);
    self->flag_1f8 = 0;
}