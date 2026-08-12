// CAIGameCameraBase::GetControlAngleXY  @ 0x00697080
// mov eax,[ecx]; sub esp,0x38; push 1.0f; lea edx,[esp+4]; push edx;
// call [eax+4]; mov ecx,eax; call 0xa0c380; add esp,0x38; ret
//
// __fastcall member: this arrives in ecx. Loads own vtable, calls slot 1
// (virtual, [eax+4]) passing a 0x38-byte scratch temp by address and 1.0f;
// the returned object pointer is the 'this' for a __fastcall float helper
// at 0xa0c380.

struct SomeObj;

// external float __fastcall helper at 0x00a0c380
float __fastcall Helper_0x00a0c380(SomeObj *self);

// 0x38-byte scratch temp (matches sub esp,0x38)
struct ScratchTemp {
    char data[0x38];
};

struct CAIGameCameraBase {
    // vtable with at least 2 slots; slot 1 (index 1) = [eax+4]
    virtual SomeObj *vf0();
    virtual SomeObj *GetSomeObj(ScratchTemp *out, float t);

    float GetControlAngleXY();
};

float CAIGameCameraBase::GetControlAngleXY()
{
    ScratchTemp tmp;
    SomeObj *o = this->GetSomeObj(&tmp, 1.0f);
    return Helper_0x00a0c380(o);
}