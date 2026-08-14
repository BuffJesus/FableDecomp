// Byte-exact reconstruction of CCategory::GetCategoryIdentifier @ 0x005da7f0
// __fastcall (modeled as __fastcall self-in-ecx)

typedef int EInventoryCategoryIdentifier;

struct CCounted;

struct CCountedVtbl {
    void (__fastcall *dtor)(CCounted*);    // slot 0
    void (__fastcall *release)(CCounted*); // slot 1 -> [vtable+4]
};

struct CCounted {
    CCountedVtbl* vtbl;   // +0x00
    int refcount;         // +0x04
    char pad[0x24];       // +0x08 .. +0x2b
    int identifier;       // +0x2c

    void AddRef() { ++refcount; }
    void Release() { if (--refcount == 0) vtbl->release(this); }
    EInventoryCategoryIdentifier GetIdentifier() { return identifier; }
};

struct CCountedPtr {
    CCounted* p;
    CCountedPtr(CCounted* q) : p(q) { if (p) p->AddRef(); }
    ~CCountedPtr() { p->Release(); }
    CCounted* operator->() { return p; }
};

struct CCategory {
    char pad[0xc];        // +0x00 .. +0x0b
    CCounted* counted;    // +0x0c
};

EInventoryCategoryIdentifier __fastcall GetCategoryIdentifier(CCategory* self)
{
    CCountedPtr tmp(self->counted);
    return tmp->GetIdentifier();
}