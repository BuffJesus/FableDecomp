#pragma once
#include "fable_display_viewport.h"
struct FableNativeViewport
{
    unsigned long x,y,width,height;
    float minZ,maxZ;
};
struct FableViewportDevice;
struct FableViewportDeviceVTable
{
    void* beforeSetViewport[47];
    long (__stdcall *SetViewport)(FableViewportDevice*,const FableNativeViewport*);
};
struct FableViewportDevice { FableViewportDeviceVTable* vtable; };
FABLE_STATIC_ASSERT(sizeof(FableNativeViewport)==24);
FABLE_STATIC_ASSERT(offsetof(FableViewportDeviceVTable,SetViewport)==0xbc);
