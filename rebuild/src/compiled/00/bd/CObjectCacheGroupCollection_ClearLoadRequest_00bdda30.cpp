struct CEngineStaticMapFileBlockManager {
    void Release(void* block);  // __fastcall -> rewritten to __fastcall(this): ecx=this, stack arg
};

struct CObjectCacheGroupCollection {
    char pad0[0x34];
    void* field_34;       // +0x34
    char pad38[0x48-0x38];
    unsigned char flags;  // +0x48
};

void __fastcall ClearLoadRequest(CObjectCacheGroupCollection* self, void* edx, CEngineStaticMapFileBlockManager* mgr)
{
    if (self->flags & 8) {
        mgr->Release(self->field_34);
        self->flags &= 0xf7;
    }
}