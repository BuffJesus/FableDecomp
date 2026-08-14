// Byte-exact reconstruction of CFileDataInputStream::~CFileDataInputStream @ 0x00994430

struct Member { void* vp; };
struct Base { void* vp; };
struct CFileDataInputStream {
    void* vptr;      // +0x00
    Member member;   // +0x04
};

// subobject destructor @ 0x99eae0 (thiscall on Member*)
extern void __fastcall Member_dtor(Member* self);
// base destructor @ 0x99a300 (thiscall on Base*)
extern void __fastcall Base_dtor(Base* self);

// vtable addresses
#define VT_DERIVED ((void*)0x0129a6f8)
#define VT_BASE    ((void*)0x0129a69c)

void __fastcall dtor(CFileDataInputStream* self)
{
    self->vptr = VT_DERIVED;
    Member_dtor(&self->member);
    self->vptr = VT_BASE;
    Base_dtor((Base*)self);
}