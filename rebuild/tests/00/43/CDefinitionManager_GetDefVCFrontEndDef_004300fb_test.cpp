// Behaviour of CDefinitionManager::GetDef<CFrontEndDef> 0x004300FB: an index
// below 1 fails without a lookup; otherwise GetPDefFromClassIndex(name, index)
// supplies a locked def; a null def fails and leaves the output alone, a def
// replaces the output's (unlocking the old one, which is deleted at zero, and
// locking the new one) unless the output already holds it; the lookup's own
// lock is always dropped.
#include "fable_definition_manager.h"
#include <stdio.h>
#include <string.h>

class CFrontEndDef;

// A def as the pointer sees it: vptr (slot 1 = DeleteCIVCountedPointeeBase)
// and the lock count at +4.
struct FakeDef
{
    virtual void Slot0() {}
    virtual void Delete() const { ++deleted; }
    mutable unsigned long count;
    mutable int deleted;
};

static int g_calls;
static const CCharString* g_name;
static long g_index;
static FakeDef* g_result;

CDefPointer<const CDefClassBase> CDefinitionManager::GetPDefFromClassIndex(const CCharString& name, long index) const
{
    ++g_calls; g_name = &name; g_index = index;
    if (g_result)
        ++g_result->count;   // the returned pointer holds a lock
    return CDefPointer<const CDefClassBase>(reinterpret_cast<const CDefClassBase*>(g_result));
}

static bool Get(const CCharString& name, long index, CDefPointer<const CFrontEndDef>* out)
{
    static unsigned char store[sizeof(CDefinitionManager)];
    return reinterpret_cast<const CDefinitionManager*>(store)->GetDef<CFrontEndDef>(name, index, out);
}

int main()
{
    unsigned char nameStore[sizeof(CCharString)];
    memset(nameStore, 0, sizeof(nameStore));
    const CCharString& name = *reinterpret_cast<CCharString*>(nameStore);
    FakeDef a, b;
    a.count = 1; a.deleted = 0; b.count = 1; b.deleted = 0;
    CDefPointer<const CFrontEndDef> out;

    // Index 0 or negative: no lookup.
    g_result = &a;
    if (Get(name, 0, &out) || Get(name, -2, &out) || g_calls != 0 || out.Object) { puts("bad index"); return 1; }

    // Not found.
    g_result = 0;
    if (Get(name, 3, &out) || g_calls != 1 || g_name != &name || g_index != 3 || out.Object) { puts("null def"); return 1; }

    // Found into an empty pointer: one lock kept.
    g_result = &a;
    if (!Get(name, 1, &out) || out.Object != &a || a.count != 2 || a.deleted) { puts("first def"); return 1; }

    // The same def again: unchanged.
    if (!Get(name, 1, &out) || out.Object != &a || a.count != 2 || a.deleted) { puts("same def"); return 1; }

    // A null result keeps the old def.
    g_result = 0;
    if (Get(name, 5, &out) || out.Object != &a || a.count != 2) { puts("null keeps"); return 1; }

    // Replaced: the old def is unlocked (and deleted at zero), the new one locked.
    b.count = 0;
    a.count = 1;
    g_result = &b;
    if (!Get(name, 2, &out) || out.Object != &b || b.count != 1 || b.deleted || a.count != 0 || a.deleted != 1)
    { puts("replace"); return 1; }

    // The lookup's lock drop deletes a def nobody else holds... never here:
    // the output always keeps one. Drop the output and check the delete.
    out.Release();
    if (b.count != 0 || b.deleted != 1) { puts("release"); return 1; }

    puts("FABLETLC_CDEFINITIONMANAGER_GETDEF_CFRONTENDDEF_004300FB_PASS");
    return 0;
}