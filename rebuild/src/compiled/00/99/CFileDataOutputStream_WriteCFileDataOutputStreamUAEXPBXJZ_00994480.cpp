// CFileDataOutputStream::Write forwarder ->File->vslot4(n, p, 0)
#include "engine/CFileDataOutputStream.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAFile {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4(const void* p, long n, int zero);
};

void __fastcall Write(CFileDataOutputStream* self, int /*edx*/, const void* p, long n)
{
    self->File->v4(p, n, 0);
}