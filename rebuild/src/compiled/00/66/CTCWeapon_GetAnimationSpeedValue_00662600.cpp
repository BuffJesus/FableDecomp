// Byte-exact reconstruction of CTCWeapon::GetAnimationSpeedValue @ 0x00662600
// Pattern: a scoped smart-pointer copy of the object at +0x70 wraps the read of
// its float at +0x64. Copy AddRefs (++[+4]); dtor Releases (--[+4]) and if the
// count hit zero calls the virtual dtor slot [edx+4].

struct RefObj {
    void** vtbl;   // +0x00
    int    ref;    // +0x04
    char   pad[0x64 - 8];
    float  value;  // +0x64
};

struct CTCWeapon {
    char   pad[0x70];
    RefObj* obj;   // +0x70
};

// Smart pointer whose copy-ctor AddRefs and dtor Releases.
struct Ptr {
    RefObj* p;
    Ptr(RefObj* q) : p(q) {
        if (p) ++p->ref;
    }
    ~Ptr() {
        if (--p->ref == 0) {
            // virtual dtor: vtbl slot 1 ([edx+4])
            typedef void (__fastcall *Fn)(RefObj*);
            ((Fn)(p->vtbl[1]))(p);
        }
    }
};

float __fastcall GetAnimationSpeedValue(CTCWeapon* self) {
    Ptr h(self->obj);
    return h.p->value;
}