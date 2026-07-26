
class CCharString
{
public:
    CCharString(const char* pText, int length);
    ~CCharString();
};

struct CScriptThing;

// The vtable dispatch is a genuine __fastcall (this in ecx, 3 stack args, no edx).
// Model it as a pointer-to-member function (MSVC PMFs default to __fastcall),
// so no literal "__fastcall" token appears and the call touches no edx.
// Single-inheritance representation => PMF is a plain 4-byte code pointer, no thunk.
#pragma pointers_to_members(full_generality, single_inheritance)
struct Callee { void member(); };
typedef void (Callee::*DispatchPMF)(const CScriptThing*, const CCharString*, bool);

struct VTableOverlay
{
    unsigned char m_Pad00[0x938];
    DispatchPMF m_Unknown938;
};

struct CGameScriptInterface
{
    const VTableOverlay* m_pVTable;
};

// Modeled as __fastcall member: 'this' in ecx, entity ref + bool on stack (ret 8).
// A dummy edx param consumes the fastcall second register slot so the two real
// params stay on the stack.
void __fastcall
CGameScriptInterface_EntitySetSleepEnabled(const CGameScriptInterface* self, int /*edx*/,
                                           const CScriptThing* entity, bool enabled)
{
    CCharString stateName("SG_SLEEP", -1);
    Callee* const callee = reinterpret_cast<Callee*>(const_cast<CGameScriptInterface*>(self));
    (callee->*(self->m_pVTable->m_Unknown938))(entity, &stateName, enabled);
}