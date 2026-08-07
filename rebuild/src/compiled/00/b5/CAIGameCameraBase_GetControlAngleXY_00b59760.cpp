#include "rebuild_abi.h"
// CAIGameCameraBase::GetControlAngleXY @ 0x00b59760
struct Loc { int v; };
struct CAIGameCameraBase {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8(); virtual void GetSub(Loc *out); // slot 9 = 0x24
    void GetControlAngleXY();
};
extern "C" void FABLE_FASTCALL FableConsume_b59760(Loc *p);
void CAIGameCameraBase::GetControlAngleXY()
{
    Loc local;
    GetSub(&local);
    FableConsume_b59760(&local);
}