#pragma once
#include "rebuild_abi.h"
class C2DBoxF;
class C2DVector;
float FABLE_FASTCALL GFAXToVX(float value); // retail 009E1DA0
float FABLE_FASTCALL GFAYToVY(float value); // retail 009E1DD0
float FABLE_FASTCALL GFVXToAX(float value); // retail 009E1E10
float FABLE_FASTCALL GFVYToAY(float value); // retail 009E1E40
float FABLE_FASTCALL GFXToY(float x); // retail 009E23A0
float FABLE_FASTCALL GFYToX(float y); // retail 009E23F0
void FABLE_FASTCALL GFACoordToVCoord(const C2DVector&, C2DVector&); // retail 009E21D0
void FABLE_FASTCALL GFVBoxToABox(const C2DBoxF*,C2DBoxF*); // retail 009E1E70
void FABLE_FASTCALL GFABoxToVBox(const C2DBoxF&,C2DBoxF&); // retail 009E2020
// Native virtual-coordinate storage. Names describe observed roles; PDB data
// names for the two per-pixel scales have not been recovered.
extern double FableVirtualPixelScaleX_013ca800;
extern double FableVirtualPixelScaleY_013ca808;
extern double g_FableVirtualWidth_013961E8;
extern double g_FableVirtualHeight_013961F0;
