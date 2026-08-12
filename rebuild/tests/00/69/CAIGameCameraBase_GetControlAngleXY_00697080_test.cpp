#include <stdio.h>

struct SomeObj;
struct ScratchTemp { char data[0x38]; };

struct SomeObj {
    float val;
};

float __fastcall Helper_0x00a0c380(SomeObj *self)
{
    return self->val;
}

static SomeObj g_obj;

struct CAIGameCameraBase {
    float storedT;
    ScratchTemp *gotBuf;
    virtual SomeObj *vf0() { return 0; }
    virtual SomeObj *GetSomeObj(ScratchTemp *out, float t)
    {
        storedT = t;
        gotBuf = out;
        g_obj.val = 42.5f;
        return &g_obj;
    }
    float GetControlAngleXY();
};

float CAIGameCameraBase::GetControlAngleXY()
{
    ScratchTemp tmp;
    SomeObj *o = this->GetSomeObj(&tmp, 1.0f);
    return Helper_0x00a0c380(o);
}

int main()
{
    CAIGameCameraBase cam;
    cam.storedT = 0.0f;
    cam.gotBuf = 0;
    float r = cam.GetControlAngleXY();
    bool ok = (r == 42.5f) && (cam.storedT == 1.0f) && (cam.gotBuf != 0);
    if (ok) printf("GETCTRLANGLE_OK\n");
    else printf("FAIL r=%f t=%f\n", r, cam.storedT);
    return ok ? 0 : 1;
}