class CLandscapeLayerMesh
{
public:
    CLandscapeLayerMesh(const CEngineLandscapeMap* pMap)
    {
        struct CLandscapeLayerMeshOverlay
        {
            const CEngineLandscapeMap* MapAt00;   // 0x00, observed in constructor
            void* vfptr;                          // 0x04, observed in constructor
            long ForegroundTextureIndex;          // 0x08
            long BackgroundTextureIndex;          // 0x0C
            std::uint16_t SelfIlluminationLo;     // 0x10
            std::uint16_t SelfIlluminationHi;     // 0x12
            CGraphicInfoFrame* ForegroundTexture; // 0x14
            CGraphicInfoFrame* BackgroundTexture; // 0x18
            CGraphicInfoFrame* BumpMap;           // 0x1C
        };

        static_assert(offsetof(CLandscapeLayerMeshOverlay, MapAt00) == 0x00);
        static_assert(offsetof(CLandscapeLayerMeshOverlay, vfptr) == 0x04);
        static_assert(offsetof(CLandscapeLayerMeshOverlay, ForegroundTextureIndex) == 0x08);
        static_assert(offsetof(CLandscapeLayerMeshOverlay, BackgroundTextureIndex) == 0x0C);
        static_assert(offsetof(CLandscapeLayerMeshOverlay, SelfIlluminationLo) == 0x10);
        static_assert(offsetof(CLandscapeLayerMeshOverlay, SelfIlluminationHi) == 0x12);
        static_assert(offsetof(CLandscapeLayerMeshOverlay, ForegroundTexture) == 0x14);
        static_assert(offsetof(CLandscapeLayerMeshOverlay, BackgroundTexture) == 0x18);
        static_assert(offsetof(CLandscapeLayerMeshOverlay, BumpMap) == 0x1C);

        auto* self = reinterpret_cast<CLandscapeLayerMeshOverlay*>(this);
        self->MapAt00 = pMap;
        self->vfptr = PTR_DAT_0139c900;
        self->ForegroundTextureIndex = 0;
        self->BackgroundTextureIndex = 0;
        self->SelfIlluminationLo = 0;
        self->SelfIlluminationHi = 0;
        self->ForegroundTexture = nullptr;
        self->BackgroundTexture = nullptr;
        self->BumpMap = nullptr;
    }
};