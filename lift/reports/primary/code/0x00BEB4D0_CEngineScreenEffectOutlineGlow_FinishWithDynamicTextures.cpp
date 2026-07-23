struct CEngineLandscapeGlobalsOverlay
{
    std::byte _pad0[0x6B0];
    CEngineLandscapeMemoryManager* MemoryManager;
};
static_assert(offsetof(CEngineLandscapeGlobalsOverlay, MemoryManager) == 0x6B0);

extern CEngineLandscapeGlobalsOverlay* g_EngineLandscapeGlobals;
extern void LandscapeLayerList__InsertNode(CTexture* texture);

void CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(unsigned int freeFlags)
{
    *reinterpret_cast<void**>(this) = reinterpret_cast<void*>(0x012A8134);

    if (this->Blur2Texture != nullptr)
    {
        ::operator delete(this->Blur2Texture);
        this->Blur2Texture = nullptr;
    }

    if (this->Blur1Texture != nullptr)
    {
        LandscapeLayerList__InsertNode(this->Blur1Texture);
        this->Blur1Texture = nullptr;
    }

    *reinterpret_cast<void**>(this) = reinterpret_cast<void*>(0x0129B860);

    if ((freeFlags & 1U) != 0)
    {
        CEngineLandscapeMemoryManager::FreeVirtual(g_EngineLandscapeGlobals->MemoryManager, this);
    }
}