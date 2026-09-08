#include "engine/CEngine.h"

struct CEngine_Deletable : CEngine { virtual ~CEngine_Deletable(); };
void __fastcall CEngine_Release(CEngine_Deletable* engine) { delete engine; }
