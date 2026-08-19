// CDefinitionManager::GetPDefFromGlobalIndex (overload 'b') @ 0x009ada10 (44 bytes).
//
// Genuine thiscall member returning CDefPointer<CDefClassBase> BY VALUE.
// MSVC7.1 passes the hidden return-value pointer as the FIRST explicit stack
// argument (this stays in ecx; retptr at [esp+4]; index at [esp+8]), and the
// callee cleans both stack args on return (ret 8).
//
// Body: ensures the def at globalIndex is resident (LoadDef), then looks it
// up in the manager's global index->entry table (this+0xa4), pulls the
// CDefClassBase* out of the entry (entry+0xc), stores it into the returned
// CDefPointer (a raw pointer at offset 0), and if non-null bumps the def's
// refcount (def+0x4), matching CDefPointer's copy/attach semantics.

struct CDefClassBase
{
    long pad0;      // +0x00
    long RefCount;  // +0x04
};

struct CDefTableEntry
{
    long pad0;                 // +0x00
    long pad1;                 // +0x04
    long pad2;                 // +0x08
    CDefClassBase* pDef;        // +0x0C
};

struct CDefPointer_CDefClassBase
{
    CDefClassBase* Ptr;

    // Attach-and-addref constructor: writes the pointer into the (hidden,
    // caller-supplied) return slot and bumps the def's refcount if non-null.
    // Written this way so MSVC7.1 builds the CDefPointer directly in the
    // caller's retval slot (NRVO) instead of through a named local, which is
    // what makes the retptr store land BETWEEN the two register pops, exactly
    // as retail does.
    CDefPointer_CDefClassBase(CDefClassBase* p) : Ptr(p)
    {
        if (Ptr)
            ++Ptr->RefCount;
    }
};

struct CDefinitionManager
{
    char pad[0xa4];
    CDefTableEntry** GlobalIndexTable; // +0xa4

    void LoadDef(unsigned long globalIndex);
    CDefPointer_CDefClassBase GetPDefFromGlobalIndex_b(long globalIndex);
};

CDefPointer_CDefClassBase CDefinitionManager::GetPDefFromGlobalIndex_b(long globalIndex)
{
    LoadDef((unsigned long)globalIndex);
    return CDefPointer_CDefClassBase(GlobalIndexTable[globalIndex]->pDef);
}