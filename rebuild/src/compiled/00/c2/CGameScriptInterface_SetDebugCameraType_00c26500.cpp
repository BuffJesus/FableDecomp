// Byte-exact reconstruction of retail 0x00c26500 (CGameScriptInterface::SetDebugCameraType leaf)
// Actual body computes an unsigned division of a span by a virtual-call divisor.

struct Divisor;

struct DivisorVtbl {
    unsigned int (__fastcall *slot0)(Divisor* self);
    unsigned int (__fastcall *slot1)(Divisor* self);
    unsigned int (__fastcall *slot2)(Divisor* self);  // called via [edx+8]
};

struct Divisor {
    DivisorVtbl* vtbl;
};

struct Holder {
    unsigned int slot0;  // +0x00
    Divisor* obj;        // +0x04 ; accessed as [eax+4]
};

struct Self {
    char pad0[0x14];
    unsigned int lo;     // +0x14
    char pad1[0x1c - 0x18];
    unsigned int hi;     // +0x1c
    Holder* holder;      // +0x20
};

unsigned int __fastcall SetDebugCameraType(Self* self)
{
    unsigned int span = self->hi - self->lo;
    Divisor* d = self->holder->obj;
    unsigned int divisor = d->vtbl->slot2(d);
    return span / divisor;
}