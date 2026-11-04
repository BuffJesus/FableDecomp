#include "fable_engine.h"
inline CIEngine::~CIEngine() {}
CEngine::~CEngine() { Uninitialise(); }
void CEngine::Release() { delete this; }
