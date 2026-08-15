// Byte-exact reconstruction of CMiniMapDisplay::ClearPrimitiveHandles @ 0x00443cf0

struct ElemA {            // 0x10 bytes, dtor operates on +0xc
    char pad[0xc];
    int  member_c;
};

// element of second vector: virtual, 8 bytes. slot0 = thiscall(int)
struct VBase {
    virtual void slot0(int x) = 0;
    int other;           // +4 (after vptr at +0)
};

struct SubObj28 { char pad[4]; };  // subobject at this+0x28

struct CMiniMapDisplay {
    char     pad00[0x1c];
    VBase*   vecB_begin;   // +0x1c
    VBase*   vecB_end;     // +0x20
    char     pad24[0x28 - 0x24];
    SubObj28 sub28;        // +0x28
    char     pad2c[0x154 - 0x2c];
    ElemA*   vecA_begin;   // +0x154
    ElemA*   vecA_end;     // +0x158
};

// element dtor 0x99eae0 (thiscall)
extern "C" void __fastcall ElemA_dtor(int self);
// operator delete 0xbfea14 (cdecl)
extern "C" void __cdecl op_delete(void* p);
// subobject clear 0x442610 (thiscall)
extern "C" void __fastcall Sub28_clear(SubObj28* self);

void __fastcall ClearPrimitiveHandles(CMiniMapDisplay* self)
{
    ElemA* ebx = self->vecA_end;
    ElemA* esi = self->vecA_begin;
    if (esi != ebx) {
        do {
            ElemA_dtor((int)&esi->member_c);
            esi = (ElemA*)((char*)esi + 0x10);
        } while (esi != ebx);
    }
    ElemA* a = self->vecA_begin;
    if (a) {
        op_delete(a);
    }
    Sub28_clear(&self->sub28);

    VBase* ebx2 = self->vecB_end;
    VBase* esi2 = self->vecB_begin;
    if (esi2 != ebx2) {
        do {
            esi2->slot0(0);
            esi2 = (VBase*)((char*)esi2 + 8);
        } while (esi2 != ebx2);
    }
    VBase* b = self->vecB_begin;
    if (b) {
        op_delete(b);
    }
}