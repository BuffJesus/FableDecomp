#include "fable_registry.h"

CSystemRegistry::~CSystemRegistry()
{
    if (openKey_ != 0)
        g_FableRegCloseKey_0143FBFC(openKey_);
}
