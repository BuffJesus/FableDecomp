#pragma once
#include <stddef.h>
#include "engine/CGraphicDataBank.h"
class CTexture;
class CCharString;
class CEngineTextureMorph;
class CIEngineLongSet;
// Partial retail view. Donor CompletedBankFile is +41c; retail B2AD80
// tail-forwards to +398 and B2AD90 reaches its async base member at +4fc.
// The embedded bank view and this class are not allocation-size claims.
class CEngineSurfaceCompositionManager {
public:
    unsigned long AddCustomMorphTexture(CTexture&,const CCharString&);
    void UpdateDiskCache(const CArray<CEngineTextureMorph>&,bool,float,const CIEngineLongSet*);
    void ChangeSceneForTextureBank();
    unsigned char UnrecoveredPrefix[0x398];
    CGraphicDataBank CompletedBankFile;
};
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager,CompletedBankFile)==0x398);
