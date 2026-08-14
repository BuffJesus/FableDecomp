// Byte-exact reconstruction of CGameScriptInterface::AddGossipVillages @ 0x008aa330
// impl 0x8ae890 takes the CCharString BY VALUE; the caller copy-constructs the temp,
// the callee owns/destroys it. The thunk then destroys its own 'val' parameter.

struct CCharString {
    void* data;
    CCharString(const CCharString& o);
    ~CCharString();
};

struct CGameScriptInterface { int dummy; };

// By-value param => caller copy-constructs into the arg slot, CALLEE destroys it,
// so no destructor for the copy is emitted in this thunk (only 'val' is destroyed).
void __fastcall Impl_AddGossipVillages(CCharString byVal);

void __fastcall CGameScriptInterface_AddGossipVillages(CGameScriptInterface* self, void*, CCharString val)
{
    (void)self;
    Impl_AddGossipVillages(val);
}