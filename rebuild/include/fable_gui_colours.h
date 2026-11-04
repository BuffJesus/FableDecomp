#pragma once
#include "fable_rgb_colour.h"
#include "rebuild_abi.h"
#include <stddef.h>

// Ego_r.pdb CGuiColoursInfo; retail InitSystemColours confirms every offset.
class CGuiColoursInfo
{
public:
    CRGBColour BackgroundShadow3D;
    CRGBColour ButtonFace;
    CRGBColour ButtonHighlight;
    CRGBColour ButtonLight;
    CRGBColour ButtonShadow;
    CRGBColour ButtonText;
    CRGBColour ActiveBorder;
    CRGBColour ActiveWindowLeftSideCaption;
    CRGBColour ActiveWindowRightSideCaption;
    CRGBColour InactiveWindowLeftSideCaption;
    CRGBColour InactiveWindowRightSideCaption;
    CRGBColour DesktopColour;
    CRGBColour CaptionText;
    CRGBColour CaptionInactiveText;
    CRGBColour GrayText;
    CRGBColour HighlightItem;
    CRGBColour HighlightText;
    CRGBColour TooltipBackground;
    CRGBColour TooltipText;
    CRGBColour MenuBackground;
    CRGBColour MenuText;
    CRGBColour Scrollbar;
    CRGBColour WindowBackground;
    CRGBColour WindowFrame;
    CRGBColour WindowText;
};
FABLE_STATIC_ASSERT(sizeof(CGuiColoursInfo)==0x64);
FABLE_STATIC_ASSERT(offsetof(CGuiColoursInfo,BackgroundShadow3D)==0x00);
FABLE_STATIC_ASSERT(offsetof(CGuiColoursInfo,ButtonFace)==0x04);
FABLE_STATIC_ASSERT(offsetof(CGuiColoursInfo,ButtonHighlight)==0x08);
FABLE_STATIC_ASSERT(offsetof(CGuiColoursInfo,ButtonLight)==0x0c);
FABLE_STATIC_ASSERT(offsetof(CGuiColoursInfo,ButtonShadow)==0x10);
FABLE_STATIC_ASSERT(offsetof(CGuiColoursInfo,ButtonText)==0x14);
FABLE_STATIC_ASSERT(offsetof(CGuiColoursInfo,ActiveBorder)==0x18);
FABLE_STATIC_ASSERT(offsetof(CGuiColoursInfo,ActiveWindowLeftSideCaption)==0x1c);
FABLE_STATIC_ASSERT(offsetof(CGuiColoursInfo,ActiveWindowRightSideCaption)==0x20);
FABLE_STATIC_ASSERT(offsetof(CGuiColoursInfo,InactiveWindowLeftSideCaption)==0x24);
FABLE_STATIC_ASSERT(offsetof(CGuiColoursInfo,InactiveWindowRightSideCaption)==0x28);
FABLE_STATIC_ASSERT(offsetof(CGuiColoursInfo,DesktopColour)==0x2c);
FABLE_STATIC_ASSERT(offsetof(CGuiColoursInfo,CaptionText)==0x30);
FABLE_STATIC_ASSERT(offsetof(CGuiColoursInfo,CaptionInactiveText)==0x34);
FABLE_STATIC_ASSERT(offsetof(CGuiColoursInfo,GrayText)==0x38);
FABLE_STATIC_ASSERT(offsetof(CGuiColoursInfo,HighlightItem)==0x3c);
FABLE_STATIC_ASSERT(offsetof(CGuiColoursInfo,HighlightText)==0x40);
FABLE_STATIC_ASSERT(offsetof(CGuiColoursInfo,TooltipBackground)==0x44);
FABLE_STATIC_ASSERT(offsetof(CGuiColoursInfo,TooltipText)==0x48);
FABLE_STATIC_ASSERT(offsetof(CGuiColoursInfo,MenuBackground)==0x4c);
FABLE_STATIC_ASSERT(offsetof(CGuiColoursInfo,MenuText)==0x50);
FABLE_STATIC_ASSERT(offsetof(CGuiColoursInfo,Scrollbar)==0x54);
FABLE_STATIC_ASSERT(offsetof(CGuiColoursInfo,WindowBackground)==0x58);
FABLE_STATIC_ASSERT(offsetof(CGuiColoursInfo,WindowFrame)==0x5c);
FABLE_STATIC_ASSERT(offsetof(CGuiColoursInfo,WindowText)==0x60);
