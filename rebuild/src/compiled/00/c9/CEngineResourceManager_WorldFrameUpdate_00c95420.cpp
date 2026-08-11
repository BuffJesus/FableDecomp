// CEngineResourceManager::WorldFrameUpdate @ 0x00c95420
// Retail body:
//   if (this->field30) { this->Sub_c95370(); g_afterUpdate(this); }

struct CEngineResourceManager
{
    char pad0[0x30];
    void* field30;   // +0x30

    void Sub_c95370();          // non-virtual member @ 0xc95370
    void WorldFrameUpdate();
};

// global cdecl function pointer at 0x1440154
extern void (__cdecl *g_afterUpdate)(CEngineResourceManager*);
void (__cdecl *g_afterUpdate)(CEngineResourceManager*) = 0;

void CEngineResourceManager::WorldFrameUpdate()
{
    if (this->field30 != 0)
    {
        this->Sub_c95370();
        g_afterUpdate(this);
    }
}