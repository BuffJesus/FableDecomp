#pragma once
#include "fable_d3d9.h"
#include "rebuild_abi.h"
#if !defined(_D3D9_H_)
typedef enum _D3DQUERYTYPE { D3DQUERYTYPE_EVENT=8 } D3DQUERYTYPE;
#define D3DISSUE_END 1
#define D3DGETDATA_FLUSH 1
struct IDirect3DQuery9 : public IUnknown {
 virtual HRESULT __stdcall GetDevice(IDirect3DDevice9** device)=0;
 virtual D3DQUERYTYPE __stdcall GetType()=0;
 virtual DWORD __stdcall GetDataSize()=0;
 virtual HRESULT __stdcall Issue(DWORD flags)=0;
 virtual HRESULT __stdcall GetData(void* data,DWORD size,DWORD flags)=0;
};
#endif
struct FableD3DQueryDeviceVTable {
 void* Prefix[118];
 HRESULT (__stdcall *CreateQuery)(IDirect3DDevice9*,D3DQUERYTYPE,IDirect3DQuery9**);
};
struct FableD3DQueryDevice {FableD3DQueryDeviceVTable* VTable;};
FABLE_STATIC_ASSERT(offsetof(FableD3DQueryDeviceVTable,CreateQuery)==0x1D8);
