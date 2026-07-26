struct CLandscapeLayerMeshOverlay
{
    void* vfptr;                                              // 0x00
    long ForegroundTextureIndex;                              // 0x04
    long BackgroundTextureIndex;                              // 0x08
    long BumpMapTextureIndex;                                 // 0x0C
    float SelfIllumination;                                   // 0x10
    CGraphicInfoFrame* ForegroundTexture;                     // 0x14
    CGraphicInfoFrame* BackgroundTexture;                     // 0x18
    CGraphicInfoFrame* BumpMap;                               // 0x1C
    long MinTextureMipMap;                                    // 0x20
    long MinBumpMapMipMap;                                    // 0x24
    LANDSCAPE_TEXTURE_MAPPING_DIRECTION MappingDirection;      // 0x28
    long ForegroundTextureMipmapLevel;                        // 0x2C
    long ForegroundBumpmapTextureMipmapLevel;                 // 0x30
    CIndexBuffer* IndexBuffer;                                // 0x34
    CVertexBufferWin32* ForegroundVertexBuffer;               // 0x38
    CLandscapeLayerMesh* Next;                                // 0x3C
    CEngineLandscapeMap* Map;                                 // 0x40
    unsigned short VertexCount;                               // 0x44
    unsigned short PolyCountIncDegenerates;                   // 0x46
    bool UsesSharedIndexBuffer;                               // 0x48
    std::byte pad49[0x03];
    CVertexBufferWin32* ProceduralTextureVertexBuffer;        // 0x4C
    CVertex* Vertices;                                        // 0x50
};

static_assert(offsetof(CLandscapeLayerMeshOverlay, ForegroundTextureIndex) == 0x04);
static_assert(offsetof(CLandscapeLayerMeshOverlay, BackgroundTextureIndex) == 0x08);
static_assert(offsetof(CLandscapeLayerMeshOverlay, BumpMapTextureIndex) == 0x0C);
static_assert(offsetof(CLandscapeLayerMeshOverlay, SelfIllumination) == 0x10);
static_assert(offsetof(CLandscapeLayerMeshOverlay, ForegroundTexture) == 0x14);
static_assert(offsetof(CLandscapeLayerMeshOverlay, BackgroundTexture) == 0x18);
static_assert(offsetof(CLandscapeLayerMeshOverlay, BumpMap) == 0x1C);
static_assert(offsetof(CLandscapeLayerMeshOverlay, MinTextureMipMap) == 0x20);
static_assert(offsetof(CLandscapeLayerMeshOverlay, MinBumpMapMipMap) == 0x24);
static_assert(offsetof(CLandscapeLayerMeshOverlay, MappingDirection) == 0x28);
static_assert(offsetof(CLandscapeLayerMeshOverlay, ForegroundTextureMipmapLevel) == 0x2C);
static_assert(offsetof(CLandscapeLayerMeshOverlay, ForegroundBumpmapTextureMipmapLevel) == 0x30);
static_assert(offsetof(CLandscapeLayerMeshOverlay, IndexBuffer) == 0x34);
static_assert(offsetof(CLandscapeLayerMeshOverlay, ForegroundVertexBuffer) == 0x38);
static_assert(offsetof(CLandscapeLayerMeshOverlay, Next) == 0x3C);
static_assert(offsetof(CLandscapeLayerMeshOverlay, Map) == 0x40);
static_assert(offsetof(CLandscapeLayerMeshOverlay, VertexCount) == 0x44);
static_assert(offsetof(CLandscapeLayerMeshOverlay, PolyCountIncDegenerates) == 0x46);
static_assert(offsetof(CLandscapeLayerMeshOverlay, UsesSharedIndexBuffer) == 0x48);
static_assert(offsetof(CLandscapeLayerMeshOverlay, ProceduralTextureVertexBuffer) == 0x4C);
static_assert(offsetof(CLandscapeLayerMeshOverlay, Vertices) == 0x50);
static_assert(sizeof(CLandscapeLayerMeshOverlay) == 0x54);

CLandscapeLayerMesh::CLandscapeLayerMesh(CEngineLandscapeMap* pMap)
{
    auto* self = reinterpret_cast<CLandscapeLayerMeshOverlay*>(this);

    self->Map = pMap;
    self->vfptr = &PTR__vector_deleting_destructor__012a835c;
    self->Next = nullptr;
    self->VertexCount = 0;
    self->PolyCountIncDegenerates = 0;
    self->ForegroundTextureIndex = 0;
    self->BackgroundTextureIndex = 0;
    self->BumpMapTextureIndex = 0;
    self->ForegroundTexture = nullptr;
    self->BackgroundTexture = nullptr;
    self->BumpMap = nullptr;
    self->MappingDirection = static_cast<LANDSCAPE_TEXTURE_MAPPING_DIRECTION>(0);
    self->IndexBuffer = nullptr;
    self->ForegroundVertexBuffer = nullptr;
    self->UsesSharedIndexBuffer = false;
    self->ForegroundBumpmapTextureMipmapLevel = 0;
    self->ForegroundTextureMipmapLevel = 0;
    self->MinTextureMipMap = 0;
    self->MinBumpMapMipMap = 0;
}