#include "fable_iengine.h"
#include "fable_string.h"
inline CIEngine::~CIEngine() {}
bool CIEngine::Initialise(const CEngineInit&) { return CInitBaseClass::Initialise(); }
void CIEngine::Release() { delete this; }
void CIEngine::SetSeaBodyToRender(CCharString name) {}
namespace { void EmitDestructor(CIEngine* engine) { engine->CIEngine::~CIEngine(); } }
