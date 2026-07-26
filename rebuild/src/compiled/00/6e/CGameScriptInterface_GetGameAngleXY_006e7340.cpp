// CGameScriptInterface::GetGameAngleXY  @ 006e7340
extern float g_defaultGameAngleXY;

// Obj: getAngle at vtable +0x64 => slot index 25 (0..24 fillers)
struct Obj {
    virtual void f00(); virtual void f01(); virtual void f02(); virtual void f03();
    virtual void f04(); virtual void f05(); virtual void f06(); virtual void f07();
    virtual void f08(); virtual void f09(); virtual void f10(); virtual void f11();
    virtual void f12(); virtual void f13(); virtual void f14(); virtual void f15();
    virtual void f16(); virtual void f17(); virtual void f18(); virtual void f19();
    virtual void f20(); virtual void f21(); virtual void f22(); virtual void f23();
    virtual void f24();
    virtual float getAngle(); // +0x64
};

// Holder: tryGet at vtable +0x28 => slot index 10 (0..9 fillers)
struct Holder {
    virtual void g00(); virtual void g01(); virtual void g02(); virtual void g03();
    virtual void g04(); virtual void g05(); virtual void g06(); virtual void g07();
    virtual void g08(); virtual void g09();
    virtual bool tryGet(Obj** out); // +0x28
};

struct Inner {
    char pad34[0x34];
    Holder* holder;
};

struct CGameScriptInterface {
    void* pad0;
    Inner* inner;
};

float __fastcall CGameScriptInterface_GetGameAngleXY(CGameScriptInterface* self)
{
    Obj* local;
    Holder* h = self->inner->holder;
    if (h->tryGet(&local)) {
        return local->getAngle();
    }
    return g_defaultGameAngleXY;
}