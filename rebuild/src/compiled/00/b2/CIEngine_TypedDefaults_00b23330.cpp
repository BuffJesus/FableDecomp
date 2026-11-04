#include "fable_iengine.h"
#include "fable_rgb_colour.h"
bool CIEngine::SetVideoOptions(const CEngineVideoOptions& options) { return 0; }
bool CIEngine::CheckPrimitiveVisibility(unsigned __int64 id) { return 1; }
float CIEngine::GetPrimitiveDrawDistanceFactor() const { return 1.0f; }
bool CIEngine::ClipSphere(const C3DBoundingSphere& sphere) const { return 1; }
bool CIEngine::ClipBox(const C3DBoundingBox& box) const { return 1; }
unsigned long CIEngine::AddChildPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned long flags) { return 0; }
void CIEngine::CollectPrimitiveStatsForArea(CIEnginePrimitiveStatMap& stats, const C2DBoxF& area, EEnginePrimitiveStatIndex_t index, unsigned long flags) const {  }
void CIEngine::ProcessPrimitiveStats(const CIEnginePrimitiveStatMap& stats, EEnginePrimitiveStatIndex_t index, unsigned long& total, CArray<CCharString>& lines) const {  }
const C3DAnimationInfo* CIEngine::GetEditorAnimation(const CWideString& name) const { return 0; }
unsigned long CIEngine::AddCustomMorphTexture(CTexture& texture, const CCharString& name) const { return 0; }
bool CIEngine::IsStaticMapNeedingUpdate(long map, const CCharString& name, unsigned long checksum) const { return 0; }
void CIEngine::OnActivate() { Active=true; }
void CIEngine::OnDeactivate() { Active=false; }
bool CIEngine::IsActive() const { return Active; }
void CIEngine::SetLandscapeDebugTintColour(long map, CRGBColour colour) {  }
void CIEngine::UpdateTextureMorphCache(const CArray<CEngineTextureMorph>& morphs, bool force, float time, const CIEngineLongSet* used) {  }
CTexture* CIEngine::AllocateTextureFromEngineResourcePools(unsigned long flags, const C2DExtentsI& size, long mips, const CPixelFormat& format) { return 0; }
bool CIEngine::DeallocateShadowBuffer() { return 0; }
const CRGBFloatColour CIEngine::PeekGlobalDiffuseColour() const { return CRGBFloatColour(1,1,1,1); }
const CRGBFloatColour CIEngine::PeekGlobalAmbientColour() const { return CRGBFloatColour(1,1,1,1); }
