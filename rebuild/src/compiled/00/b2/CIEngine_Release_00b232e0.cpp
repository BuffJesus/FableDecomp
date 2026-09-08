#include "engine/CIEngine.h"

struct CIEngine_Deletable : CIEngine { virtual ~CIEngine_Deletable(); };
void __fastcall CIEngine_Release(CIEngine_Deletable* engine) { delete engine; }
