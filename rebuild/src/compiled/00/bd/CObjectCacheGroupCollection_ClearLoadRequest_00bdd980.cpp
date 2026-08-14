// Byte-exact reconstruction of CObjectCacheGroupCollection::ClearLoadRequest @ 0x00bdd980

// callee at 0xbe0400 : __fastcall member taking one pushed arg, returns void.
// Model as a real (non-virtual) member method so the compiler emits ecx=this,
// arg pushed, edx untouched — matching retail's `mov ecx,[esp+8]; push eax; call`.
struct CEngineStaticMapFileBlockManager {
    void ReleaseBlock(void* arg);
};

struct CObjectCacheGroupCollection {
    char pad0[0x28];
    void* field_28;     // +0x28
    char pad2c[0x36 - 0x2c];
    unsigned char flags; // +0x36
};

// __fastcall: self in ecx, mgr on stack. Harness rewrites __fastcall->__fastcall,
// which keeps self in ecx AND puts arg2 in edx. To force mgr onto the stack, place
// an int edx dummy as the 2nd param so mgr becomes the 3rd (stack) param.
void __fastcall ClearLoadRequest(CObjectCacheGroupCollection* self,
                                 int edx_dummy,
                                 CEngineStaticMapFileBlockManager* mgr)
{
    (void)edx_dummy;
    if (self->flags & 2) {
        mgr->ReleaseBlock(self->field_28);
        self->flags &= (unsigned char)0xfd;
    }
}