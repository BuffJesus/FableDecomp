#pragma once
#include "fable_shader_generic_light.h"

#include "rebuild_abi.h"
#include "fable_counted_pointer.h"
#include "fable_rgb_colour.h"
#include "fable_matrix4x4.h"
#include "fable_stlport_vector.h"
#include "fable_4d_vector.h"
#include "fable_2d_vector.h"
#include "fable_indexed_array.h"
#include "fable_shader_constant_layout.h"
#include "fable_simple_memory_pool.h"
#include "fable_pixel_shader.h"
#include "fable_pretransposed_bone_matrix.h"
class CVertexShaderConstantLayout;
class C4DVector;
class CVertexShader;
class CShaderResource;
class CPixelShaderData;
class CMatrix3x4;

struct FablePixelShaderDevice
{
    void** vtable;
};

struct CCountedPixelShaderView
{
    void* unknown00;
    void* pixelShader04;
};

struct CShaderRenderManagerUpdatePixelShaderView
{
    fable_u8 unknown000[0x1D4];
    FablePixelShaderDevice* displayDevice1D4;
    fable_u8 unknown1D8[0x0C];
    CCountedPixelShaderView* currentPixelShader1E4;
    fable_u8 unknown1E8[0x1F0];
    fable_u32 shaderStateMask3D8;

    void UpdatePixelShader();
};

class CRenderManager;
struct IDirect3DDevice9;
class CShaderRenderManager : public CBaseClassNonCopyable
{
public:
    CShaderRenderManager();
    virtual ~CShaderRenderManager();
    struct CLightInfo
    {
        CLightInfo():IsEnabled(false),UpdateFlag(true){}
        CShaderGenericLight Light;
        bool IsEnabled,UpdateFlag;
        fable_u8 padding36[2];
    };
    // Ego_r names, with retail offsets confirmed by 00987FE0..009881E5.
    // The donor has larger containers earlier in this object.
    const CVertexShaderConstantLayout* CurrentVSConstantLayout;
    long CurrentVSConstantLayoutIndex;
    _STL::vector<const CVertexShaderConstantLayout*> VertexShaderConstantLayouts;
    CVertexShaderConstantLayout NullConstantLayout;
    CLightInfo* Lights;
    C4DVector* LightBuffer;
    unsigned long LightCount,AllocatedLights,AllocatedLightSize;
    CSimpleMemoryPool LightPool;
    _STL::vector<C4DVector> LightGlobals;
    _STL::vector<C4DVector> LightAttenuations;
    CRGBFloatColour AmbientColour;
    CRGBFloatColour FogColour;
    CRGBColour TextureFactor;
    CRenderManager* RenderManager;
    IDirect3DDevice9* D3DDevice;
    void* CurrentVertexShader;
    void* CurrentVertexShaderVertexDeclairation; // original PDB spelling
    CPixelShader CurrentPixelShader;
    bool WorldMatrixIsIdentity;
    fable_u8 unknown1E9[7];
    CMatrix4x4 WorldMatrix;
    CMatrix4x4 ViewMatrix;
    CMatrix4x4 ProjectionMatrix;
    CMatrix4x4 ViewProjectionMatrix;
    CMatrix4x4 CombinedProjectionMatrix;
    CMatrix4x4 TextureTransforms[1];
    C4DVector FogTransform;
    C3DVector CameraPos;
    long TextureTransformCount;
    bool VertexShadersEnabled;
    bool PixelShadersEnabled;
    bool SWLightingEnable;
    bool SoftwareVertexShaderEnable;
    bool SoftwareFixedFunctionEnable;
    bool FixedFunctionVertexMode;
    fable_u8 unknown396[2];
    float MultisampleViewPortScale;
    fable_u8 unknown39C[12];
    _STL::vector<CCountedPointer<CShaderResource> > ResourceManagers;
    CIndexedArray<CCharString> PSConstNames;
    _STL::vector<C4DVector> PSConstTable;
    fable_u32 updateFlags3D8;
    _STL::vector<CPreTransposedBoneMatrix> BoneMatrices;

    CCountedPointer<CShaderResource> CreateResource();
    void OnPostViewportChanged();
    void Initialise(CRenderManager*,IDirect3DDevice9*);
    void Uninitialise();
    void EnableVertexShaders();
    void DisableVertexShaders();
    void EnablePixelShaders();
    void DisablePixelShaders();
    void ResetLight(unsigned long index);
    void SetVSConstants(long startRegister, const void* data, unsigned long vectorCount);
    void UpdatePixelShader();
    void UpdateAmbient();
    void UpdateFogColour();
    void UpdateLightGlobals();
    void UpdatePixelShaderConstants();
    void ApplyVertexShader(const CVertexShader& shader,bool force);
    void ResetWorldTransform();
    void SetWorldTransform(const CMatrix3x4& transform); // 009881F0
    void SetViewTransform(const CMatrix3x4& transform); // 00988350
    void SetProjectionTransform(const CMatrix4x4& transform); // 00988540
    void SetCameraPos(const C3DVector& position); // 00988320, formerly mislabeled SetFogColour
    void UpdateCombinedProjectionTransform(); // 00988A50; native body still pending
    void RegisterVertexShaderConstantLayout(long index,const CVertexShaderConstantLayout* layout);
    void UnregisterVertexShaderConstantLayouts();
    void SetVertexShaderConstantLayout(long index);
    void AllocateLights(unsigned long count,unsigned long lightSize);
    _STL::vector<const CVertexShaderConstantLayout*>& ConstantLayouts()
    {
        return VertexShaderConstantLayouts;
    }
    _STL::vector<C4DVector>& LightGlobalVector()
    {
        return LightGlobals;
    }
    _STL::vector<C4DVector>& LightAttenuationVector()
    {
        return LightAttenuations;
    }
    _STL::vector<CCountedPointer<CShaderResource> >& Resources()
    {
        return ResourceManagers;
    }
    CIndexedArray<CCharString>& PixelShaderConstantNames()
    {
        return PSConstNames;
    }
    _STL::vector<C4DVector>& PixelShaderConstantTable()
    {
        return PSConstTable;
    }
};

FABLE_STATIC_ASSERT(
    sizeof(CShaderRenderManagerUpdatePixelShaderView) == 0x3DC);
FABLE_STATIC_ASSERT(sizeof(CShaderRenderManager) == 0x3E8);
FABLE_STATIC_ASSERT(sizeof(CShaderRenderManager::CLightInfo) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,Lights) == 0x164);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,VertexShaderConstantLayouts) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,NullConstantLayout) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,RenderManager) == 0x1D0);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,LightGlobals) == 0x194);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,LightAttenuations) == 0x1A0);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,AmbientColour) == 0x1AC);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,FogColour) == 0x1BC);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,CurrentVertexShader) == 0x1D8);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,CurrentPixelShader) == 0x1E0);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,WorldMatrix) == 0x1F0);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,ViewMatrix) == 0x230);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,ProjectionMatrix) == 0x270);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,ViewProjectionMatrix) == 0x2B0);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,CombinedProjectionMatrix) == 0x2F0);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,TextureTransforms) == 0x330);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,FogTransform) == 0x370);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,CameraPos) == 0x380);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,TextureTransformCount) == 0x38C);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,VertexShadersEnabled) == 0x390);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,PixelShadersEnabled) == 0x391);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,PSConstTable) == 0x3CC);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,ResourceManagers) == 0x3A8);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,PSConstNames) == 0x3B4);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,updateFlags3D8) == 0x3D8);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager,BoneMatrices) == 0x3DC);

void FABLE_FASTCALL CShaderRenderManager_ResetWorldTransform(
    CShaderRenderManager* shaderManager);

// Address verified by CDisplayManager::Init and its destructor.
extern CShaderRenderManager FableShaderRenderManager_013bc470;
