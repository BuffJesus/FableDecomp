#include <stdlib.h>

void __cdecl operator delete(void* memory);

struct CatchCleanupEntry
{
    virtual void Destroy(int flags);
    unsigned long unknown;
};

struct CatchCleanupOverlay
{
    unsigned char unused[0x1c];
    CatchCleanupEntry* begin;
    CatchCleanupEntry* end;
};

void __fastcall Catch_0047e8cc(CatchCleanupOverlay* self)
{
    if (self != 0) {
        CatchCleanupEntry* finish = self->end;
        for (CatchCleanupEntry* current = self->begin; current != finish; ++current) {
            current->Destroy(0);
        }
        if (self->begin != 0) {
            free(self->begin);
        }
        operator delete(self);
    }
}