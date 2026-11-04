#pragma once
#include <stddef.h>

// CRenderStateManager / CRenderManagerCore: the bases of the retail 2D render
// manager (RTTI CRenderManager2D : CRenderManagerCore : CRenderStateManager;
// one object, GPRenderManager2D retail 0x013B8384). Ego_r.pdb names and
// layout; retail agrees on every offset used so far:
//   * StateStack (8-byte CStateStackElement) +0x08, StateUpdateList +0x2008,
//     StateStackSize +0x2808, StateUpdateListSize +0x280C,
//     CurrentBookmarkMask +0x2814 (RealiseRenderState 0x00A058C0 and the
//     inlined setters in CNewFrontendGameComponent::Draw 0x0042DF9E);
//   * DeviceState +0x2818 (CDeviceState::FogEnable +0x12C -> +0x2944,
//     TextureMipMapMode[0] +0x62C -> +0x2E44, same Draw);
//   * CRenderManagerCore::D3DDeviceCaps +0x3A58 (GFInitialise 0x004022B0).
// PDB/RTTI root is CBaseClassNonCopyable; all established offsets remain unchanged.

#include "fable_d3d9.h"
#include "fable_d3d9_state_types.h"
#include "rebuild_abi.h"
#include "fable_base_class.h"
#include "fable_surface_types.h"
#include "fable_system_font.h"

// Ego_r.pdb EPrimitiveType: values are D3DPRIMITIVETYPE's.
enum EPrimitiveType
{
    PT_NULL = -1,
    PT_POINT_LIST = 1,
    PT_LINE_LIST = 2,
    PT_LINE_STRIP = 3,
    PT_TRIANGLE_LIST = 4,
    PT_TRIANGLE_STRIP = 5,
    PT_TRIANGLE_FAN = 6
};

class C2DBoxF;
class C2DBoxI;
class CDisplayManager;
class CIndexBuffer;
class C3DVector;
class CRGBColour;

// FableWin.pdb enum TEXTURE_MIPMAP_MODE.
enum TEXTURE_MIPMAP_MODE
{
    TEXTURE_MIPMAP_POINT = 0,
    TEXTURE_MIPMAP_LINEAR = 1,
    TEXTURE_MIPMAP_DISABLE = 2,
    TEXTURE_MIPMAP_MODE_COUNT = 3
};

enum ALPHA_TEST_FUNC { ALPHA_TEST_NEVER, ALPHA_TEST_LESS, ALPHA_TEST_EQUAL, ALPHA_TEST_LESS_EQUAL, ALPHA_TEST_GREATER, ALPHA_TEST_NOT_EQUAL, ALPHA_TEST_GREATER_EQUAL, ALPHA_TEST_ALWAYS, ALPHA_TEST_MODE_COUNT };

enum MATERIAL_COLOUR_SOURCE { MATERIAL_COLOUR_SOURCE_MATERIAL, MATERIAL_COLOUR_SOURCE_COLOUR1, MATERIAL_COLOUR_SOURCE_COLOUR2, MATERIAL_COLOUR_SOURCE_COUNT };
enum TEXTURE_OP_MODE {
    TEXTURE_OP_DISABLE, TEXTURE_OP_SELECT_ARG1, TEXTURE_OP_SELECT_ARG2,
    TEXTURE_OP_MODULATE, TEXTURE_OP_MODULATE_2X, TEXTURE_OP_MODULATE_4X,
    TEXTURE_OP_ADD, TEXTURE_OP_ADD_SIGNED, TEXTURE_OP_ADD_SIGNED2X,
    TEXTURE_OP_SUBTRACT, TEXTURE_OP_ADD_SMOOTH, TEXTURE_OP_BLEND_DIFFUSE_ALPHA,
    TEXTURE_OP_BLEND_TEXTURE_ALPHA, TEXTURE_OP_BLEND_FACTOR_ALPHA,
    TEXTURE_OP_BLEND_TEXTURE_ALPHA_PM, TEXTURE_OP_BLEND_CURRENT_ALPHA,
    TEXTURE_OP_PREMODULATE, TEXTURE_OP_MODULATE_ALPHA_ADD_COLOR,
    TEXTURE_OP_MODULATE_COLOR_ADD_ALPHA, TEXTURE_OP_MODULATE_INV_ALPHA_ADD_COLOR,
    TEXTURE_OP_MODULATE_INV_COLOR_ADD_ALPHA, TEXTURE_OP_BUMP_ENV_MAP,
    TEXTURE_OP_BUMP_ENV_MAP_LUMINANCE, TEXTURE_OP_DOT_PRODUCT3, TEXTURE_OP_MODE_COUNT
};
class CRenderStateManager : public CBaseClassNonCopyable
{
    friend class CStaticFontBank; // Narrow access for typed font-state reconstruction.
    friend class CStreamingFontBank; // Same, for the streaming font (PrepareToRender 0x009A1720 / Render).
    friend class CShaderRenderManager;
    friend class CRenderManager2D;
    friend class CStateBlockFunctionSold;
    friend class CStateBlockFunctionAlphaAdditive;
    friend class CStateBlockFunctionAlphaMultiply;
    friend class CStateBlockFunctionAlphaBoolean;
    friend class CStateBlockFunctionAlphaText;
    friend struct CRenderStateManagerTestAccess;
    void SetupRenderStates(); // 00A04630
public:
    explicit CRenderStateManager(unsigned long flags);
    virtual ~CRenderStateManager();
    void Init(IDirect3DDevice9* device); // 00A07900
    void SetDefaultRenderStates(); // 00A059E0
    void RealiseRenderState(); // 00A058C0
    void RestoreCaptureBlock(); // 00A05840
    void StartCaptureBlock()
    {
        StateStack[StateStackSize].State = &BookmarkDummyState;
        ++StateStackSize;
        CurrentBookmarkMask <<= 1;
    }
    void ForceResendRenderStatesToHardware(); // 00A044E0
    // Ego_r.pdb CRenderStateManager::CStateInfo (0x14).
    struct CStateInfo
    {
        CStateInfo() : CurrentState(0), DesiredState(0), BookmarkMask(0), DirtyListFlag(false), StateType(0) {}
        unsigned long CurrentState;       // +0x00
        unsigned long DesiredState;       // +0x04
        unsigned long BookmarkMask;       // +0x08
        unsigned long StateParam;         // +0x0C (Ego_r: a union)
        bool DirtyListFlag;               // +0x10
        unsigned char StateType;          // +0x11
        unsigned char TextureStage;       // +0x12
    };

    // Ego_r.pdb CRenderStateManager::CStateStackElement (8).
    struct CStateStackElement
    {
        CStateInfo* State;
        unsigned long Value;
    };

    // Ego_r.pdb CRenderStateManager::CDeviceState (0x120C); only the states
    // reconstructed callers set are named.
    struct CDeviceState
    {
        CDeviceState();
        CStateInfo PolyShadeMode; // +0x00
        CStateInfo PolyFillMode; // +0x14
        CStateInfo ColourWriteEnable; // +0x28
        CStateInfo ZBufferWriteEnable; // +0x3c
        CStateInfo ZBufferEnable; // +0x50
        CStateInfo ZBufferFunc; // +0x64
        CStateInfo PolyCullMode; // +0x78
        CStateInfo AlphaTestFunc; // +0x8c
        CStateInfo AlphaBlendEnable; // +0xa0
        CStateInfo AlphaTestEnable; // +0xb4
        CStateInfo AlphaRef; // +0xc8
        CStateInfo VertexBlendMode; // +0xdc
        CStateInfo SoftwareVertexProcessing; // +0xf0
        CStateInfo SourceAlphaBlendMode; // +0x104
        CStateInfo DestAlphaBlendMode; // +0x118
        CStateInfo FogEnable; // +0x12c
        CStateInfo FogColour; // +0x140
        CStateInfo FogStart; // +0x154
        CStateInfo FogEnd; // +0x168
        CStateInfo FogDensity; // +0x17c
        CStateInfo FogMode; // +0x190
        CStateInfo FogVertexMode; // +0x1a4
        CStateInfo TextureFactor; // +0x1b8
        CStateInfo SpecularEnable; // +0x1cc
        CStateInfo AntialiasingOn; // +0x1e0
        CStateInfo DitherEnable; // +0x1f4
        CStateInfo ClippingEnable; // +0x208
        CStateInfo LightingEnable; // +0x21c
        CStateInfo NormalizeNormals; // +0x230
        CStateInfo AmbientMaterialColourSource; // +0x244
        CStateInfo DiffuseMaterialColourSource; // +0x258
        CStateInfo SpecularMaterialColourSource; // +0x26c
        CStateInfo PointSpriteEnable; // +0x280
        CStateInfo PointScaleEnable; // +0x294
        CStateInfo PointSize; // +0x2a8
        CStateInfo PointSizeMin; // +0x2bc
        CStateInfo PointSizeMax; // +0x2d0
        CStateInfo PointScaleConstantFactor; // +0x2e4
        CStateInfo PointScaleDistanceFactor; // +0x2f8
        CStateInfo PointScaleDistanceSqrFactor; // +0x30c
        CStateInfo ShadowFunc; // +0x320
        CStateInfo SolidOffsetEnable; // +0x334
        CStateInfo DepthOffset; // +0x348
        CStateInfo DepthOffsetSlope; // +0x35c
        CStateInfo StencilEnable; // +0x370
        CStateInfo StencilOpFail; // +0x384
        CStateInfo StencilOpZFail; // +0x398
        CStateInfo StencilOpPass; // +0x3ac
        CStateInfo StencilFunc; // +0x3c0
        CStateInfo StencilRef; // +0x3d4
        CStateInfo StencilMask; // +0x3e8
        CStateInfo StencilWriteMask; // +0x3fc
        CStateInfo StippleEnable; // +0x410
        CStateInfo BlendOpMode; // +0x424
        CStateInfo BlendColour; // +0x438
        CStateInfo TextureCoordWrapMode[8]; // +0x44c
        CStateInfo TextureMaxAnisotropy[8]; // +0x4ec
        CStateInfo TextureMipmapLODBias[8]; // +0x58c
        CStateInfo TextureMipMapMode[8]; // +0x62c
        CStateInfo TextureUVModeU[8]; // +0x6cc
        CStateInfo TextureUVModeV[8]; // +0x76c
        CStateInfo TextureUVModeW[8]; // +0x80c
        CStateInfo TextureFilterModeMin[8]; // +0x8ac
        CStateInfo TextureFilterModeMag[8]; // +0x94c
        CStateInfo TextureOperation[8]; // +0x9ec
        CStateInfo TextureAlphaOperation[8]; // +0xa8c
        CStateInfo TextureArgument1[8]; // +0xb2c
        CStateInfo TextureArgument2[8]; // +0xbcc
        CStateInfo TextureAlphaArgument1[8]; // +0xc6c
        CStateInfo TextureAlphaArgument2[8]; // +0xd0c
        CStateInfo TextureCoordIndex[8]; // +0xdac
        CStateInfo TextureTransformMode[8]; // +0xe4c
        CStateInfo TextureBorderColour[8]; // +0xeec
        CStateInfo TextureBumpEnvMat00[8]; // +0xf8c
        CStateInfo TextureBumpEnvMat01[8]; // +0x102c
        CStateInfo TextureBumpEnvMat10[8]; // +0x10cc
        CStateInfo TextureBumpEnvMat11[8]; // +0x116c
    };

    __forceinline void SetAmbientMaterialColourSource(MATERIAL_COLOUR_SOURCE source) { SetRenderState(D3DMaterialColourSourceTable[source],DeviceState.AmbientMaterialColourSource); }
    __forceinline void SetDiffuseMaterialColourSource(MATERIAL_COLOUR_SOURCE source) { SetRenderState(D3DMaterialColourSourceTable[source],DeviceState.DiffuseMaterialColourSource); }
    __forceinline void SetTextureOperation(long stage,TEXTURE_OP_MODE mode) { SetRenderState(D3DTextureOperationModeTable[mode],DeviceState.TextureOperation[stage]); }
    __forceinline void SetTextureAlphaOperation(long stage,TEXTURE_OP_MODE mode) { SetRenderState(D3DTextureOperationModeTable[mode],DeviceState.TextureAlphaOperation[stage]); }
    __forceinline void SetColourWriteEnable(bool enable) { SetRenderState(enable?15:0,DeviceState.ColourWriteEnable); }
    __forceinline void SetZBufferWriteEnable(bool enable) { SetRenderState(enable,DeviceState.ZBufferWriteEnable); }
    __forceinline void SetAlphaBlendEnable(bool enable) { SetRenderState(enable,DeviceState.AlphaBlendEnable); }
    __forceinline void SetAlphaTestFunc(ALPHA_TEST_FUNC mode) { SetRenderState(D3DAlphaFuncModeTable[mode],DeviceState.AlphaTestFunc); }
    __forceinline void SetAlphaTestEnable(bool enable) { SetRenderState(enable,DeviceState.AlphaTestEnable); }
    // FableWin ?SetFogEnable@CRenderStateManager@@QAEX_N@Z; inline in retail.
    __forceinline void SetFogEnable(bool enable)
    {
        SetRenderState(enable, DeviceState.FogEnable);
    }
    __forceinline void SetNormalizeNormals(bool enable)
    {
        SetRenderState(enable, DeviceState.NormalizeNormals);
    }

    // FableWin ?SetTextureMipMapMode@CRenderStateManager@@QAEXJW4TEXTURE_MIPMAP_MODE@@@Z;
    // inline in retail (Draw 0x0042DF9E reads the table entry at 0x01396E68
    // for stage 0 / mode 1).
    __forceinline void SetTextureMipMapMode(long stage, TEXTURE_MIPMAP_MODE mode)
    {
        SetRenderState(D3DTextureMipMapModeTable[mode], DeviceState.TextureMipMapMode[stage]);
    }

private:
    void SetupRenderState(CStateInfo& state,D3DRENDERSTATETYPE type); // 00A04580
    void SetupTextureStageState(CStateInfo& state,D3DTEXTURESTAGESTATETYPE type,long stage); // 00A045B0
    void SetupSamplerStageState(CStateInfo& state,D3DSAMPLERSTATETYPE type,long stage); // 00A045F0
    // FableWin ?SetRenderState@CRenderStateManager@@AAEXKAAUCStateInfo@1@@Z;
    // inline in retail. A changed state is bookmarked once (its old value
    // pushed on the state stack), then queued for RealiseRenderState. The
    // FableWin body's stack-overflow / bookmark asserts are debug-only.
    __forceinline void SetRenderState(unsigned long value, CStateInfo& info)
    {
        if (info.DesiredState != value)
        {
            if ((CurrentBookmarkMask & info.BookmarkMask) == 0)
            {
                info.BookmarkMask |= CurrentBookmarkMask;
                StateStack[StateStackSize].State = &info;
                StateStack[StateStackSize].Value = info.DesiredState;
                ++StateStackSize;
            }
            info.DesiredState = value;
            AddStateToUpdateList(info);
        }
    }

    // FableWin ?AddStateToUpdateList@CRenderStateManager@@AAEXAAUCStateInfo@1@@Z;
    // inline in retail.
    __forceinline void AddStateToUpdateList(CStateInfo& info)
    {
        if (!info.DirtyListFlag)
        {
            info.DirtyListFlag = true;
            StateUpdateList[StateUpdateListSize++] = &info;
        }
    }

    // ?D3DTextureMipMapModeTable@CRenderStateManager@@0PAKA (retail 0x01396E64).
    static unsigned long D3DTextureMipMapModeTable[];
    static unsigned long D3DAlphaFuncModeTable[ALPHA_TEST_MODE_COUNT];
    static unsigned long D3DMaterialColourSourceTable[MATERIAL_COLOUR_SOURCE_COUNT];
    static unsigned long D3DTextureOperationModeTable[TEXTURE_OP_MODE_COUNT];

    void* PD3DDevice;                                // +0x04 IDirect3DDevice9*
    CStateStackElement StateStack[1024];             // +0x08
    CStateInfo* StateUpdateList[256];                // +0x2008
    CStateInfo* AllStatesList[256];                  // +0x2408
    long StateStackSize;                             // +0x2808
    long StateUpdateListSize;                        // +0x280C
    long AllStatesListSize;                          // +0x2810
    unsigned long CurrentBookmarkMask;               // +0x2814
    CDeviceState DeviceState;                        // +0x2818
    CStateInfo BookmarkDummyState;                   // +0x3A24
    unsigned long InitFlags;                         // +0x3A38
};

class CMatrix3x4;
class CRenderManagerCore : public CRenderStateManager
{
public:
    CRenderManagerCore(CDisplayManager& display,unsigned long flags);
    virtual ~CRenderManagerCore();
    void Init(IDirect3DDevice9* device); // 00A0A940
    void Uninit(); // 00A0A9D0
    void WaitWhileResourceIsBusy(IDirect3DResource9* resource,float timeout); // 00A0AA70, retail no-op
    void WaitWhileGPUIsBusy(); // 00A0AF80, original PC no-op
    // ?SetVWindow@CRenderManagerCore@@QAEXABVC2DBoxF@@@Z (retail 0x00A0B560;
    // caller CNewFrontendGameComponent::Draw 0x0042DF9E).
    void SetVWindow(const C2DBoxF& window);
    void SetViewTransform(const CMatrix3x4& transform); // 00A0B0A0
    void SetWorldTransform(const CMatrix3x4& transform); // 00A0B140
    void SetWorldTransform(unsigned long index,const CMatrix3x4& transform); // 00A0B1E0 (D3DTS_WORLDMATRIX(index))
    void SetTextureTransform(unsigned long stage,const CMatrix3x4& transform); // 00A0B280
    void SetPerspectiveTransform(float nearZ,float farZ,float zStart,float zEnd); // 00A0B320
    void SetPerspectiveTransform(float e11,float e12,float e13,float e14,float e21,float e22,float e23,float e24,
        float e31,float e32,float e33,float e34,float e41,float e42,float e43,float e44); // 00A0B3E0
private:
    void SetD3DMatrix(D3DMATRIX& matrix,float e11,float e12,float e13,float e14,float e21,float e22,float e23,float e24,
        float e31,float e32,float e33,float e34,float e41,float e42,float e43,float e44); // 00A0AAD0
public:
    void SetAWindow(const C2DBoxF& window); // retail 00A0AA80
    void SetAWindow(const C2DBoxI& window); // retail 00A0AA90
    const C2DBoxF& GetCurrentAWindow() const; // retail 00A0AAA0
    const C2DBoxI& GetCurrentAWindowInt() const; // retail 00A0AAB0
    bool IsRendering(); // retail 00A0AA60
    // Empty on PC retail (fixed-function lights and the texture manager budget
    // are not used): 00A0A9F0 / 00A0AA00 / 00A0AA50 / 00A0AAC0.
    void SetDirectionalLight(long index,const CRGBColour& diffuse,const CRGBColour& specular,const C3DVector& direction);
    void SetPointLight(long index,const CRGBColour& diffuse,const CRGBColour& specular,const C3DVector& position,
        float range,float attenuation0,float attenuation1,float attenuation2);
    void DiscardTextureManagerBytes(unsigned long bytes);
    void OnPostViewportChanged();
    bool BeginRender(); // retail 00A0B480
    void EndRender(); // retail 00A0B520
    void SetMaterial(const CRGBColour& diffuse,const CRGBColour& ambient); // retail PC no-op 00A0AA10
    void EnableXBoxShaderConstants(bool enabled); // retail 00A0AA20, shipping no-op
    void DrawAsIndexedTris(const void* vertices,long stride,long vertexCount,void* indices,long primitiveCount); // 00A0AF40
    void DrawAsIndexedTris(const CTexture** textures,unsigned long textureCount,const void* vertices,long stride,long vertexCount,void* indices,long primitiveCount); // 00A0AEA0
    void SetVertexFormat(unsigned long vertexFormat); // 00A0AA30
    void DrawCurrentBuffersAsTris(unsigned long startVertex,unsigned long primitiveCount); // 00A0AE10
    // Retail 00A0AD40 / 00A0AE40 / 00A0AE70. The fan and point-list forms
    // always start at vertex 0; their first argument is unused in retail.
    void DrawCurrentBuffersIndexed(unsigned long minVertex,unsigned long vertexCount,unsigned long startIndex,unsigned long primitiveCount);
    void DrawCurrentBuffersIndexed(const CTexture** textures,unsigned long textureCount,unsigned long minVertex,unsigned long vertexCount,unsigned long startIndex,unsigned long primitiveCount); // 00A0ACA0
    void DrawCurrentBuffersAsTriFan(long vertexCount,long primitiveCount);
    void DrawCurrentBuffersAsPointList(long vertexCount,long primitiveCount);
    void DrawCurrentBuffersAsIndexedTris(unsigned long minVertex,unsigned long vertexCount,unsigned long startIndex,unsigned long primitiveCount); // 00A0AC60
    void DrawCurrentBuffersAsTris(const CTexture** textures,unsigned long textureCount,unsigned long startVertex,unsigned long primitiveCount); // 00A0ABE0
    void DrawCurrentBuffersNonIndexed(const CTexture** textures,unsigned long textureCount,unsigned long startVertex,unsigned long primitiveCount,EPrimitiveType primitiveType); // 00A0AD80
    void DrawCurrentBuffersAsIndexedTris(const CTexture** textures,unsigned long textureCount,unsigned long minVertex,unsigned long vertexCount,unsigned long startIndex,unsigned long primitiveCount); // 00A0AB50
    void AttachTextureToStage(long stage,const CTexture* texture); // 009A0CF0
    void AttachD3DTextureToStage(long stage,IDirect3DBaseTexture9* texture);

public:
    CDisplayManager* DisplayManager;
    float LastRenderEndTime;
    long NoTrisPerSecond;
    CPixelFormat ScreenPixelFormat;
    CSystemFont SystemFont;
    bool Rendering, ResolutionReinit, ResolutionSet, TexturesEnabled;

public:
    // Ego_r CRenderManagerCore::D3DDeviceCaps; GFInitialise 0x004022B0 clamps
    // its texture limits to MaxTextureWidth/Height (+0x3AB0/+0x3AB4).
    D3DCAPS9 D3DDeviceCaps;                          // +0x3A58
    D3DCAPS9 D3DDeviceSoftwareCaps;
    long TextureStagesInUse;
    unsigned long NoZSortedPolys, NoNonZSortedPolys, NoRenderStateChanges, NoPolys;
    long HighestMipmapToRender, NoTrisDrawn, NoVerticesDrawn, NoDrawPrimCalls;
    unsigned long NoLastFrameZSortedPolys, NoLastFrameNonZSortedPolys, NoLastFramePolys;
    unsigned long NoLastFrameUncachedTextureUploads, NoLastFrameCachedTextureUploads;
    IDirect3DDevice9* PD3DDevice;
    CIndexBuffer* PSelectedIndexBuffer;
    unsigned long CurrentVertexFormat;
    bool XBoxShaderConstantsEnabled, FixedFunctionPipelineEnabled;
    IDirect3DBaseTexture9* CurrentlySelectedTextures[8];
};

FABLE_STATIC_ASSERT(sizeof(CRenderStateManager::CStateInfo) == 0x14);
FABLE_STATIC_ASSERT(sizeof(CRenderStateManager::CDeviceState) == 0x120C);
FABLE_STATIC_ASSERT(sizeof(CRenderStateManager) == 0x3A3C);

FABLE_STATIC_ASSERT(sizeof(CRenderManagerCore)==0x3d20);
FABLE_STATIC_ASSERT(offsetof(CRenderManagerCore,DisplayManager)==0x3a3c);
FABLE_STATIC_ASSERT(offsetof(CRenderManagerCore,SystemFont)==0x3a4c);
FABLE_STATIC_ASSERT(offsetof(CRenderManagerCore,PD3DDevice)==0x3cf0);

// Retail current-manager pointer; original global name not recovered.
extern CRenderStateManager* FableCurrentRenderStateManager_013cb508;

// Retail table used for eight texture-coordinate wrap registrations.
extern D3DRENDERSTATETYPE FableRenderWrapStateTypes_01396da8[8];
