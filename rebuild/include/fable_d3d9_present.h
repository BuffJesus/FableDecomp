#pragma once
#include "fable_d3d9.h"
#if !defined(_WINDOWS_) && !defined(_D3D9_H_)
#define D3DDEVCAPS_PUREDEVICE 0x00100000L
#define D3DCREATE_FPU_PRESERVE 0x00000002L
#define D3DCREATE_MULTITHREADED 0x00000004L
#define D3DCREATE_PUREDEVICE 0x00000010L
#define D3DCREATE_SOFTWARE_VERTEXPROCESSING 0x00000020L
#define D3DCREATE_HARDWARE_VERTEXPROCESSING 0x00000040L
// d3d9types.h: 56-byte x86 presentation parameters, including all padding-free fields.
typedef enum _D3DSWAPEFFECT { D3DSWAPEFFECT_DISCARD=1, D3DSWAPEFFECT_FORCE_DWORD=0x7fffffff } D3DSWAPEFFECT;
struct _D3DPRESENT_PARAMETERS_
{
    UINT BackBufferWidth, BackBufferHeight;
    D3DFORMAT BackBufferFormat;
    UINT BackBufferCount;
    D3DMULTISAMPLE_TYPE MultiSampleType;
    DWORD MultiSampleQuality;
    D3DSWAPEFFECT SwapEffect;
    HWND__* hDeviceWindow;
    BOOL Windowed, EnableAutoDepthStencil;
    D3DFORMAT AutoDepthStencilFormat;
    DWORD Flags;
    UINT FullScreen_RefreshRateInHz, PresentationInterval;
};
#endif
