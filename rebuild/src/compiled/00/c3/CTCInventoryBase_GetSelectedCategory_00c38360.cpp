struct CCategory;

struct CTCInventoryBase {
    CCategory* f0;   // +0x00
    CCategory* f4;   // +0x04
};

// __fastcall in sig; ecx=self, edx=other. Harness rewrites __fastcall->__fastcall.
// Retail zeroes only AL on the mismatch path -> the real return is a byte (bool).
bool __fastcall CTCInventoryBase_GetSelectedCategory(CTCInventoryBase* self, CTCInventoryBase* other)
{
    if (self->f4 != other->f4)
        return false;
    return self->f0 == other->f0;
}