// CTCActionUseBuyHouse::OnCreate  @ 007b2a90
// void __fastcall OnCreate(CTCActionUseBuyHouse* this)

struct SomeInner;               // object at this->f4->f70
struct SomeOwner;               // object at this->f4

// external member function called at 0x7b44e0 on this->f4->f70 with &this->f0c
struct Vec3 { float x, y, z; };

struct SomeInner {
    // non-virtual member; VC7.1 direct call
    void DoThing(Vec3* v);
};

struct SomeOwner {
    char pad0[0x70];
    SomeInner* f70;
};

// base class
struct CTCActionUseBase {
    void OnCreate();            // 0x686870 base
};

struct CTCActionUseBuyHouse {
    void* f0;                   // +0
    SomeOwner* f4;             // +4
    char pad8[4];              // +8
    Vec3 f0c;                  // +0xc
    void OnCreate();
};

void CTCActionUseBuyHouse::OnCreate()
{
    this->f4->f70->DoThing(&this->f0c);
    ((CTCActionUseBase*)this)->OnCreate();
}