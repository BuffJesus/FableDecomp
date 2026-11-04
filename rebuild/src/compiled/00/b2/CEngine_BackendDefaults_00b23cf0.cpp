#include "fable_engine.h"
#include "fable_rgb_colour.h"
bool CEngine::ConvertScreenPosToWorldPos(const C2DVector& screen, C3DVector& world) const { return 0; }
bool CEngine::PreChangeResolution() { return 1; }
bool CEngine::PostChangeResolution(unsigned long width, unsigned long height, unsigned long depth) { return 1; }
long CEngine::GetStaticMapUpdatePassCount() { return 2; }
void CEngine::StartStaticMapUpdatePass(const CWideString& file, long pass, const CStaticMapGenerationVisibiilityInfo& info, const CArray<CCharString>& names) {  }
bool CEngine::IsStaticMapNeedingUpdate(long map, const CCharString& name, unsigned long checksum) const { return 0; }
void CEngine::UpdateStaticMap(long map, const CCharString& name, unsigned long checksum, long pass) {  }
void CEngine::AreaChanged(const C2DBoxI& area) {  }
void CEngine::OnActivate() { Active=true; }
void CEngine::SetLandscapeDebugTintColour(long map, CRGBColour colour) {  }
void CEngine::ReloadLocalDetailDefs() {  }
void CEngine::UpdateDebugOverlay(long overlay, const CRGBColour* colours, const C2DBoxI& area) {  }
void CEngine::EnableDebugOverlay(long overlay, bool enable, bool redraw) {  }
void CEngine::SetRenderTargetTexture(const CTexture* texture) {  }
