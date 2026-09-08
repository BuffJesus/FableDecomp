#include "engine/CEngine.h"

struct CRGBFloatColour {
    float Red, Green, Blue, Alpha;
};

// Retail-only wrapper proven by the load through GEngine + 0x3c.
struct CEngineGlobalStateRetail {
    unsigned char EarlierState[0x3c];
    CRGBFloatColour* GlobalDiffuseColour;
};
extern CEngineGlobalStateRetail* GEngine;

struct CEngineMethods : CEngine {
    CRGBFloatColour PeekGlobalDiffuseColour() const;
};

CRGBFloatColour CEngineMethods::PeekGlobalDiffuseColour() const
{
    return *GEngine->GlobalDiffuseColour;
}
