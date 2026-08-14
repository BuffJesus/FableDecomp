// Byte-exact reconstruction of CAIStateGroup_FollowPlayer::OnActivate @ 0x0094ef70
// Signature: ?OnActivate@CAIStateGroup_FollowPlayer@@UAEXXZ  (public virtual void OnActivate(void))

// call 0x4978a0 : ecx=Obj*, one pushed float arg; returns float in st0 (__fastcall member)
struct Obj {
    float Method(float v);
};

// call 0x8fcfd0 : returns Obj* in eax, takes self in ecx (__fastcall-ish, no args)
extern Obj* __fastcall GetObj(void* self);

extern float g_bias; // dword ptr [0x1293344]

struct CAIStateGroup_FollowPlayer {
    // implicit vptr at offset 0 (4 bytes)
    char pad[0x18];   // 0x04 .. 0x1b
    float f1c;        // 0x1c
    virtual void OnActivate();
};

void CAIStateGroup_FollowPlayer::OnActivate()
{
    Obj* o = GetObj(this);
    this->f1c = o->Method(2.6f) + g_bias;
}