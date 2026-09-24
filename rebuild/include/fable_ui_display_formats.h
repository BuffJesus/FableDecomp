#pragma once
#include "fable_ui_display_services.h"

// Retail 009BE5xx..009BE8xx query IDirect3D9::CheckDeviceFormat (slot 10).
struct FableUiD3D9;
typedef long (__stdcall *FableUiCheckDeviceFormat)(FableUiD3D9*,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned);
struct FableUiD3D9Vtable { void* Unrecovered00[10]; FableUiCheckDeviceFormat CheckDeviceFormat; };
struct FableUiD3D9 { FableUiD3D9Vtable* Vtable; };
struct FableUiDisplayFormatView
{
    unsigned char Unrecovered00[0x54];
    FableUiD3D9* D3D;
    void* D3DDevice;
    unsigned DeviceType,AdapterOrdinal; // First two D3DCAPS9 members.
    unsigned char Unrecovered64[0x194-0x64];
    FableUiDisplayExtent RenderTargetDimensions;
    unsigned char Unrecovered19C[0x1C4-0x19C];
    unsigned CurrentModeFormat;
};
FABLE_STATIC_ASSERT(offsetof(FableUiDisplayFormatView,D3D)==0x54);
FABLE_STATIC_ASSERT(offsetof(FableUiDisplayFormatView,RenderTargetDimensions)==0x194);
FABLE_STATIC_ASSERT(offsetof(FableUiDisplayFormatView,CurrentModeFormat)==0x1C4);
struct FableUiPixelFormatDescription
{
    unsigned D3DFormat;
    int Type,Bits,Alpha,Red,Green,Blue,Stencil,FloatingPoint;
};
FABLE_STATIC_ASSERT(sizeof(FableUiPixelFormatDescription)==36);
// 0129BA40, terminated by Bits==-1. The recovered selectors preserve table order.
extern FableUiPixelFormatDescription FableUiPixelFormats[64];
void __fastcall FableUiSetPixelFormat(int*,void*,unsigned);
extern FableUiSystemManagerView FableUiSystemManagerState;
inline bool FableUiSupportsFormat(FableUiDisplayFormatView* display,unsigned format,unsigned usage)
{
    FableUiD3D9* d3d=display->D3D;
    return d3d->Vtable->CheckDeviceFormat(d3d,display->AdapterOrdinal,display->DeviceType,
        display->CurrentModeFormat,usage,3,format)>=0;
}
