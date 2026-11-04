// Behaviour fixture for CDefClassBase's own members: the ten base defaults,
// InternalSaveBinary and UnloadResource (CDefinitionManager::UnloadDef
// expanded). The members are the real compiled providers; the manager is raw
// storage with a real instantiated-def table, the stream a recording subclass.
#include <new>
#include "fable_def_class_base.h"
#include "fable_definition_manager.h"
#include "fable_data_output_stream.h"
#include <stdio.h>
#include <string.h>

// ---- stand-ins -------------------------------------------------------------
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CCharString::CCharString(const char*, long) { storage_ = 0; }
CCharString::~CCharString() {}
CResource::CResource() : ResourceList(0), PrevResource(0), NextResource(0), ResourceSize(0x1234), LastUsedFrame(0) {}
static int g_deleted; static const void* g_deletedObject;
void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase() const { ++g_deleted; g_deletedObject = this; }
void CResourceList::RemoveResource(CResource&) {}
// Landed separately (0x009ACB20); only the vtable references it here.
void CDefClassBase::InternalLoadBinary(CDataInputStream&) {}
static unsigned char g_bytes[8]; static int g_written;
void CDataOutputStream::WriteUBYTE(unsigned char value) { g_bytes[g_written++] = value; }
struct RecordingStream : public CDataOutputStream
{
    virtual void SetPosition(unsigned long) {}
    virtual unsigned long GetPosition() const { return 0; }
    virtual void Close() {}
    virtual void Write(const void*, long) {}
};

struct TestDef : public CDefClassBase
{
    virtual void Transfer(CPersistContext&) {}
    virtual void Copy(const CDefClassBase*) {}
    virtual unsigned long GetSizeofClass() const { return sizeof(TestDef); }
};
struct ManagerAccess : public CDefinitionManager
{
    static _STL::vector<CInstantiatedDefInfo*>& Defs(CDefinitionManager* m) { return static_cast<ManagerAccess*>(m)->InstantiatedDefs; }
};

static int g_fail;
#define CHECK(c, code) do { if (!(c)) { printf("DEF_CLASS_BASE_FAIL %d line %d\n", code, __LINE__); g_fail = 1; } } while (0)

int main()
{
    int cases = 0;
    // The base defaults, called through the virtual table, leave the def untouched.
    for (int flags = 0; flags < 8; ++flags) {
        TestDef def; def.Setup = flags & 1; def.Template = (flags >> 1) & 1; def.DefaultValsApplied = (flags >> 2) & 1;
        def.GlobalIndex = 0x55aa0000 + flags;
        CDefClassBase* base = &def;
        unsigned char before[sizeof(TestDef)]; memcpy(before, &def, sizeof(def));
        CDefString name = base->GetInstantiationName(); CHECK(name.TablePos == -1, 1);
        CDefString other; other.TablePos = 77; base->SetInstantiationName(other);
        CCharString text("sub");
        base->AddSubDef(text, 1, 2); base->RemoveSubDef(text);
        CHECK(base->GetPSubDefInfo(text) == 0, 2);
        CHECK(static_cast<const CDefClassBase*>(base)->GetPSubDefInfo(text) == 0, 3);
        CHECK(!base->IsParentDef(), 4);
        base->PostTransfer();
        base->Validate(*(const CValidateDefinitionInitBase*)&def, text);
        CHECK(base->GetPSubDefInfoMap() == 0, 5);
        // CResource's defaults still fill slots 3-9.
        CHECK(base->GetUnloadableMemory() == 0x1234 && !base->IsResourceLoaded() && !base->CanAbortResourceLoad(5) && base->GetDebugMemoryInUse() == 0x1234, 6);
        CHECK(memcmp(before, &def, sizeof(def)) == 0, 7);
        // InternalSaveBinary: one byte per flag, Setup first.
        RecordingStream stream; g_written = 0; memset(g_bytes, 0xee, sizeof(g_bytes));
        base->InternalSaveBinary(stream);
        CHECK(g_written == 3 && g_bytes[0] == (flags & 1) && g_bytes[1] == ((flags >> 1) & 1) && g_bytes[2] == ((flags >> 2) & 1) && g_bytes[3] == 0xee, 8);
        CHECK(memcmp(before, &def, sizeof(def)) == 0, 9);
        cases += 2;
    }
    // UnloadResource: a def shared beyond the manager and its caller stays; otherwise it is released.
    const unsigned long counts[] = { 0, 1, 2, 3, 4, 100, 0xffffffff };
    for (int slots = 1; slots <= 5; ++slots) for (int index = 0; index < slots; ++index) for (int c = 0; c < 8; ++c) {
        static union { double align; char bytes[sizeof(CDefinitionManager)]; } memory;
        memset(&memory, 0x5a, sizeof(memory));
        CDefinitionManager* manager = (CDefinitionManager*)&memory;
        CDefinitionManager::CInstantiatedDefInfo infos[5]; CDefinitionManager::CInstantiatedDefInfo* table[5];
        TestDef held[5];
        for (int i = 0; i < slots; ++i) { table[i] = &infos[i]; infos[i].ClassIndex = 0x100 + i; infos[i].PDef.Object = &held[i]; held[i].IVRefCount = 7; }
        _STL::vector<CDefinitionManager::CInstantiatedDefInfo*>& defs = ManagerAccess::Defs(manager);
        defs._M_start = table; defs._M_finish = table + slots; defs._M_end_of_storage._M_data = table + slots;
        bool null = c == 7; unsigned long count = null ? 0 : counts[c];
        if (null) infos[index].PDef.Object = 0; else held[index].IVRefCount = count;
        TestDef self; self.PDefManager = manager; self.GlobalIndex = index;
        g_deleted = 0; g_deletedObject = 0;
        bool result = self.UnloadResource();
        bool release = null || count <= 2;
        CHECK(result == release, 10);
        if (release) {
            CHECK(infos[index].PDef.Object == 0, 11);
            if (!null) CHECK(held[index].IVRefCount == count - 1 && g_deleted == (count == 1) && (count != 1 || g_deletedObject == &held[index]), 12);
            else CHECK(g_deleted == 0, 13);
        } else CHECK(infos[index].PDef.Object == &held[index] && held[index].IVRefCount == count && g_deleted == 0, 14);
        for (int i = 0; i < slots; ++i) if (i != index) CHECK(infos[i].PDef.Object == &held[i] && held[i].IVRefCount == 7 && infos[i].ClassIndex == (unsigned long)(0x100 + i), 15);
        for (int i = 0; i < slots; ++i) infos[i].PDef.Object = 0;  // the fixture owns these defs
        ++cases;
    }
    if (g_fail) return 1;
    printf("DEF_CLASS_BASE_BEHAVIOR_PASS scenarios=%d\n", cases);
    return 0;
}
