#include "fable_system_manager_state.h"
extern "C" __declspec(dllimport) unsigned long FABLE_STDCALL GetSysColor(int index);
namespace {
inline void SetSystemColour(CRGBColour& colour,int index)
{
    unsigned long value=GetSysColor(index);
    colour.R=static_cast<unsigned char>(value);
    colour.G=static_cast<unsigned char>(static_cast<unsigned short>(value)>>8);
    colour.B=static_cast<unsigned char>(value>>16);
    colour.A=255;
}
}
void CSystemManager::InitSystemColours()
{
    SetSystemColour(SystemColours.BackgroundShadow3D,21);
    SetSystemColour(SystemColours.ButtonFace,15);
    SetSystemColour(SystemColours.ButtonHighlight,20);
    SetSystemColour(SystemColours.ButtonLight,22);
    SetSystemColour(SystemColours.ButtonShadow,16);
    SetSystemColour(SystemColours.ButtonText,18);
    SetSystemColour(SystemColours.ActiveBorder,10);
    SetSystemColour(SystemColours.ActiveWindowRightSideCaption,27);
    SetSystemColour(SystemColours.InactiveWindowRightSideCaption,28);
    SetSystemColour(SystemColours.ActiveWindowLeftSideCaption,2);
    SetSystemColour(SystemColours.InactiveWindowLeftSideCaption,3);
    SetSystemColour(SystemColours.DesktopColour,1);
    SetSystemColour(SystemColours.CaptionText,9);
    SetSystemColour(SystemColours.CaptionInactiveText,19);
    SetSystemColour(SystemColours.GrayText,17);
    SetSystemColour(SystemColours.HighlightItem,13);
    SetSystemColour(SystemColours.HighlightText,14);
    SetSystemColour(SystemColours.TooltipBackground,24);
    SetSystemColour(SystemColours.TooltipText,23);
    SetSystemColour(SystemColours.MenuBackground,4);
    SetSystemColour(SystemColours.MenuText,7);
    SetSystemColour(SystemColours.Scrollbar,0);
    SetSystemColour(SystemColours.WindowBackground,5);
    SetSystemColour(SystemColours.WindowFrame,6);
    SetSystemColour(SystemColours.WindowText,8);
}
