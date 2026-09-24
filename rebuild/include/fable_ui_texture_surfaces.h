#pragma once
#include "fable_ui_bank_runtime.h"
#include "fable_ui_display_formats.h"
#include <string.h>

// Retail D3D9 COM slots. Keep the platform interface separate from engine layout.
struct FableUiSurfaceDescription { unsigned Format,Type,Usage,Pool,MultiSampleType,MultiSampleQuality,Width,Height; };
struct FableUiLockRectangle { int Pitch; void* Bits; };
struct FableUiRectangle { int Left,Top,Right,Bottom; };
struct FableUiSurfaceLock { unsigned Width,Height; int Pitch; void* Bits; };
struct FableUiNativeSurface;
struct FableUiNativeSurfaceVtable
{
    void* QueryInterface;
    unsigned (__stdcall *AddRef)(FableUiNativeSurface*);
    unsigned (__stdcall *Release)(FableUiNativeSurface*);
    void* Unrecovered0C[9];
    long (__stdcall *GetDescription)(FableUiNativeSurface*,FableUiSurfaceDescription*);
    long (__stdcall *Lock)(FableUiNativeSurface*,FableUiLockRectangle*,const FableUiRectangle*,unsigned);
    long (__stdcall *Unlock)(FableUiNativeSurface*);
};
struct FableUiNativeSurface { FableUiNativeSurfaceVtable* Vtable; };
struct FableUiNativeTexture;
struct FableUiNativeTextureVtable
{
    void* QueryInterface;
    unsigned (__stdcall *AddRef)(FableUiNativeTexture*);
    unsigned (__stdcall *Release)(FableUiNativeTexture*);
    void* Unrecovered0C[10];
    unsigned (__stdcall *GetLevelCount)(FableUiNativeTexture*);
    void* Unrecovered38[3];
    long (__stdcall *GetLevelDescription)(FableUiNativeTexture*,unsigned,FableUiSurfaceDescription*);
    long (__stdcall *GetSurfaceLevel)(FableUiNativeTexture*,unsigned,IDirect3DSurface9**);
};
struct FableUiNativeTexture { FableUiNativeTextureVtable* Vtable; };
struct FableUiNativeDevice;
struct FableUiNativeDeviceVtable
{
    void* Unrecovered00[23];
    long (__stdcall *CreateTexture)(FableUiNativeDevice*,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,IDirect3DTexture9**,void*);
};
struct FableUiNativeDevice { FableUiNativeDeviceVtable* Vtable; };
FABLE_STATIC_ASSERT(offsetof(FableUiNativeSurfaceVtable,GetDescription)==0x30);
FABLE_STATIC_ASSERT(offsetof(FableUiNativeTextureVtable,GetLevelCount)==0x34);
FABLE_STATIC_ASSERT(offsetof(FableUiNativeTextureVtable,GetSurfaceLevel)==0x48);
FABLE_STATIC_ASSERT(offsetof(FableUiNativeDeviceVtable,CreateTexture)==0x5C);
FABLE_STATIC_ASSERT(sizeof(FableUiSurfaceDescription)==32);
inline FableUiNativeSurface* FableUiNative(CSurface* surface)
{ return reinterpret_cast<FableUiNativeSurface*>(surface->PD3DSurface); }
inline FableUiNativeTexture* FableUiNative(CTexture* texture)
{ return reinterpret_cast<FableUiNativeTexture*>(texture->PD3DTexture); }
// Shared generated CTexture represents a packed 28-bit length / 4-bit source.
inline unsigned FableUiTextureState(const CTexture* texture)
{ unsigned state; memcpy(&state,texture->ByteLength,4); return state; }
inline void FableUiSetTextureState(CTexture* texture,unsigned state)
{ memcpy(texture->ByteLength,&state,4); }
CSurface* __fastcall FableUiCopySurface(CSurface*,void*,const CSurface*); // 009F2D60
void __fastcall FableUiAttachSurface(CSurface*,void*,IDirect3DSurface9*); // 009F2F10
FableUiSurfaceLock* __fastcall FableUiLockSurface(CSurface*,void*,FableUiSurfaceLock*,unsigned); // 009F33E0
int __fastcall FableUiPixelFormatBits(const int*,void*); // 009E3820
void __fastcall FableUiUpdateTextureByteLength(CTexture*,void*); // 009F9EE0
void __fastcall FableUiReleaseTexture(CTexture*,void*); // 009F9F70
