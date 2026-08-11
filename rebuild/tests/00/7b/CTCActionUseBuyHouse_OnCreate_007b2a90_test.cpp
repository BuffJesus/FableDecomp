#include <cstdio>

struct Vec3 { float x, y, z; };

struct SomeInner {
    int tag;
    void DoThing(Vec3* v);
};

struct SomeOwner {
    char pad0[0x70];
    SomeInner* f70;
};

struct CTCActionUseBase {
    void OnCreate();
};

struct CTCActionUseBuyHouse {
    void* f0;
    SomeOwner* f4;
    char pad8[4];
    Vec3 f0c;
    void OnCreate();
};

static int g_doThingCalled = 0;
static int g_baseCalled = 0;
static Vec3* g_lastVec = 0;
static SomeInner* g_lastInner = 0;

void SomeInner::DoThing(Vec3* v) { g_doThingCalled++; g_lastVec = v; g_lastInner = this; }
void CTCActionUseBase::OnCreate() { g_baseCalled++; }

void CTCActionUseBuyHouse::OnCreate()
{
    this->f4->f70->DoThing(&this->f0c);
    ((CTCActionUseBase*)this)->OnCreate();
}

int main()
{
    SomeInner inner; inner.tag = 42;
    SomeOwner owner; owner.f70 = &inner;
    CTCActionUseBuyHouse obj;
    obj.f0 = 0;
    obj.f4 = &owner;
    obj.f0c.x = 1.0f; obj.f0c.y = 2.0f; obj.f0c.z = 3.0f;

    obj.OnCreate();

    int ok = (g_doThingCalled == 1)
          && (g_baseCalled == 1)
          && (g_lastInner == &inner)
          && (g_lastVec == &obj.f0c);

    if (ok) printf("BUYHOUSE_ONCREATE_OK\n");
    else printf("FAIL d=%d b=%d\n", g_doThingCalled, g_baseCalled);
    return ok ? 0 : 1;
}