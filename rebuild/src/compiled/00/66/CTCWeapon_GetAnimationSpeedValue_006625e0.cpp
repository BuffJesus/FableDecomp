// CTCWeapon::GetAnimationSpeedValue @ 0x006625e0
// Reads a refcounted object ptr at self+0x70, scoped add-ref/release around
// a float read at obj+0x5c, returns the float.

struct RefObj {
    void** vtbl;   // +0x00
    int    refcnt; // +0x04
    char   pad[0x54];
    float  speed;  // +0x5c
};

struct CTCWeapon {
    char    pad[0x70];
    RefObj* obj;   // +0x70
};

// Scoped smart pointer that inlines: copy => inc refcnt; dtor => dec refcnt,
// if zero call virtual slot 1 (vtbl[1]).
struct SmartPtr {
    RefObj* p;
    SmartPtr(RefObj* q) : p(q) {
        if (p) p->refcnt++;
    }
    ~SmartPtr() {
        if (p && --p->refcnt == 0) {
            ((void (__fastcall*)(RefObj*))p->vtbl[1])(p);
        }
    }
};

float __fastcall GetAnimationSpeedValue(CTCWeapon* self) {
    SmartPtr sp(self->obj);
    return sp.p->speed;
}