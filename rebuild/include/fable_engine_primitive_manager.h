#pragma once
#include "fable_base_class.h"
#include "fable_carray.h"
#include "fable_engine_primitive.h"
#include "fable_engine_layer_renderer.h"
class CEngineScene;
class CEngineInternalPrimitiveBase;
class CEnginePrimitiveTypeList;
class CEngineSubPrimitiveBase;
class CEngineSubPrimitiveChain;
class C3DVector;
namespace NEngineShadowRenderer { class CShadowSceneContext; }
enum ERendered { RENDERED_NOTHING=0,RENDERED_BOUNDING_VOLUME=1,RENDERED_OBJECT=2 };
enum ERenderPassType;
enum ESubPrimitiveType;
class CEnginePrimitiveRenderUtil : public CBaseClassNonCopyable {};

// Partial virtual interface; recovered nonvirtual methods use the donor's
// reference-to-scene-array at +4 and enable flag at +8.
class CEnginePrimitiveManager : public CEnginePrimitiveRenderUtil
{
public:
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType);
    virtual void GetSupportedTypes(CEnginePrimitiveTypeList&);
    virtual void FrameStart();
    virtual void FrameEnd();
    virtual void RenderSubPrimitive(CEngineSubPrimitiveBase&,ERenderPassType);
    virtual void RenderBoundingVolume(CEngineSubPrimitiveBase&);
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase&,EEngineRenderLayer,CEngineSubPrimitiveChain&);
    virtual ERendered RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase&);
    virtual bool PreloadPrimitive(CEngineInternalPrimitiveBase&,const C3DVector&,unsigned long);
    virtual void Render2DPrimitive(CEngineInternalPrimitiveBase&,CEngineSubPrimitiveChain&,ESubPrimitiveType);
    virtual void RenderNonsortedPrimitives(); // Ego_r virtual slot11
    virtual void AddStaticShadowData(CEngineInternalPrimitiveBase&,NEngineShadowRenderer::CShadowSceneContext&); // Ego_r virtual slot12
    void AddPrimitive(CEngineInternalPrimitiveBase&,unsigned long);
    CArrayVector<CEngineScene*>& SceneList;
    bool Enable;
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveManager)==12);
// VC7.1 cannot form offsetof for a reference member; retail +4 is checked
// by the exact AddPrimitive body, and the following member stays at +8.
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManager,Enable)==8);
