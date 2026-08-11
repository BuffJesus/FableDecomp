// Retail 0x005d97d0  _global GetPDef -> CDefPointer<CInventoryItemDef const>
// __fastcall, struct-return: ecx = hidden return buffer, edx = long id.

struct CInventoryItemDef;

// The referenced def object: refcount lives at +4.
struct RefTarget {
    void*        vtbl_or_field0; // +0
    int          refCount;       // +4
};

// CDefPointer<T> is a single pointer.
struct CDefPointer {
    RefTarget* p; // +0
};

// Global def pointer at 0x013b8c18.
RefTarget* g_currentDef;

// Helper at 0x5d94b0 (__fastcall, takes the id in ecx). Return discarded.
void __fastcall PrepareDef(long id);

CDefPointer* __fastcall GetPDef(CDefPointer* ret, long id)
{
    PrepareDef(id);
    RefTarget* d = g_currentDef;
    ret->p = d;
    if (d)
        ++d->refCount;
    return ret;
}