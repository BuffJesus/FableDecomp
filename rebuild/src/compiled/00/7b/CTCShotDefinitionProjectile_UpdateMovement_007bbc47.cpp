#pragma optimize("s",on)
// Byte-exact reconstruction of CTCShotDefinitionProjectile::UpdateMovement @ 0x007bbc47

// object at self->m4->m60 has a vtable; called slot is byte 0x98 (index 38).
// Model as a class with 38 filler virtuals + the target virtual (real thiscall).
struct VObj {
    virtual void s00(); virtual void s01(); virtual void s02(); virtual void s03();
    virtual void s04(); virtual void s05(); virtual void s06(); virtual void s07();
    virtual void s08(); virtual void s09(); virtual void s10(); virtual void s11();
    virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15();
    virtual void s16(); virtual void s17(); virtual void s18(); virtual void s19();
    virtual void s20(); virtual void s21(); virtual void s22(); virtual void s23();
    virtual void s24(); virtual void s25(); virtual void s26(); virtual void s27();
    virtual void s28(); virtual void s29(); virtual void s30(); virtual void s31();
    virtual void s32(); virtual void s33(); virtual void s34(); virtual void s35();
    virtual void s36(); virtual void s37();
    virtual void Notify(int flag);   // slot 38 -> byte offset 0x98
};

struct M4 {
    char pad[0x60];
    VObj* m60;
};

struct CTCShotDefinitionProjectile {
    void* m0;
    M4*   m4;
};

// predicate at 0x7bf3d0: __fastcall taking self in ecx, returns bool in al
extern bool __fastcall ShouldUpdate(CTCShotDefinitionProjectile* self);

void __fastcall UpdateMovement(CTCShotDefinitionProjectile* self)
{
    if (ShouldUpdate(self)) {
        self->m4->m60->Notify(1);
    }
}