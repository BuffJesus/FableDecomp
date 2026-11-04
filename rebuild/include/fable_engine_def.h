#pragma once
#include "fable_def_class_base.h"

// Ego_r CEngineDef members; the retail parent shrinks by four bytes.
// Retail fade fields are independently read at+4C/+50 by B8FD40 and435190.
class CEngineDef : public CParentDefClassBase {
public:
    static CDefClassBase* FABLE_FASTCALL Alloc(); // retail004343F0
    float LODErrorTolerance,CharacterLODErrorTolerance,LODErrorFactor;
    float SeaHeight;
    float GamePrimitiveDefaultFadeStart,GamePrimitiveDefaultFadeRangeRatio;
    float LocalDetailDefaultFadeStart,LocalDetailDefaultFadeRangeRatio;
    float FOV_2D;
    long LocalDetailBooleanAlphaDefaultAlphaRef,DefaultPrimitiveAlphaRef;
    long TestStaticMesh,TestAnimatedMesh,TestAnim,TestGraphic;
    long InvalidTextureStandin,InvalidThemeStandin;
};
FABLE_STATIC_ASSERT(sizeof(CEngineDef)==0x80);
FABLE_STATIC_ASSERT(offsetof(CEngineDef,GamePrimitiveDefaultFadeRangeRatio)==0x50);
