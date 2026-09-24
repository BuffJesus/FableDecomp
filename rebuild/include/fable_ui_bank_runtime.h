#pragma once
#include "fable_ui_bank_factory.h"
#include "engine/CResource.h"
#include "engine/CResourceList.h"
#include "engine/CGraphicBankStateBlock.h"
#include "engine/CTexture.h"
#include "engine/CSurface.h"

struct FableUiResourceBankView { void* Vtable; CResourceList ResourceList,LoadingQueue; };
struct FableUiGraphicArray { void* Begin; void* End; void* Capacity; };
// The generated CGraphicDataBank donor header is quarantined: 0x388 vs retail
// 0x30C. Tail names are PDB-derived; these offsets follow 009FEA20/009FD4E0.
struct FableUiGraphicsBankRuntimeView
{
    FableUiGraphicsBankVtable* Vtable;
    unsigned char BankFileBase[0x160];
    FableUiResourceBankView ResourceBank;
    FableUiGraphicArray GraphicList,GraphicFrameBuffer;
    bool Initialised;
    unsigned char Unrecovered1F9[3];
    CGraphicBankStateBlock StateBlock;
    int TextureSizeWarningMaxWidth,TextureSizeWarningMaxHeight;
    unsigned UnloadFrameDelay,ReduceSizeFrameDelay,TexturePointersValidForFrames;
    int MinAvailablePreloadMemory,AvailableMemory;
    bool FreezeGraphics;
    unsigned char Unrecovered27D[3];
    FableUiBankReference TextureManager;
    int PixelFormats[11];
    CTexture BlankTextures[11];
};
FABLE_STATIC_ASSERT(sizeof(FableUiResourceBankView)==0x7C);
FABLE_STATIC_ASSERT(sizeof(FableUiGraphicsBankRuntimeView)==0x30C);
FABLE_STATIC_ASSERT(offsetof(FableUiGraphicsBankRuntimeView,ResourceBank)==0x164);
FABLE_STATIC_ASSERT(offsetof(FableUiGraphicsBankRuntimeView,Initialised)==0x1F8);
FABLE_STATIC_ASSERT(offsetof(FableUiGraphicsBankRuntimeView,TextureManager)==0x280);
FABLE_STATIC_ASSERT(offsetof(FableUiGraphicsBankRuntimeView,BlankTextures)==0x2B4);
extern void* FableUiBaseVtable; // 0129A7C4
extern void* FableUiResourceBankVtable; // 0129C814
extern void* FableUiGraphicResourceBankVtable; // 0129C938
extern void* FableUiResourceListVtable; // 0129C808
extern void* FableUiResourceVtable; // 012354A4
extern void* FableUiGraphicStateVtable; // 0129C888
extern void* FableUiSurfaceVtable; // 0122F84C
extern FableUiGraphicsBankVtable FableUiGraphicsBankRuntimeVtable; // 0129C8CC
void* __fastcall FableUiConstructBase(void*,void*); // 0099A2F0
FableUiResourceBankView* __fastcall FableUiConstructResourceBank(FableUiResourceBankView*,void*); // 009FC5F0
void __fastcall FableUiResetTextureManager(FableUiBankReference*,void*,void*); // 00A002E0
void __fastcall FableUiDeleteTextureManager(void*); // 009FFD20
// Recovered dependencies shared with the texture-manager construction gate.
void* __fastcall FableUiConstructTextureManager(void*,void*); // 00A6A360, retail allocation 0x5D4
void __fastcall FableUiSetBankPreloadPolicy(void*,void*,bool); // 009D5230: empty retail body
// Recovered CBankFileAsync constructor and device-facing texture/surface methods.
void* __fastcall FableUiConstructBankFileBase(void*,void*); // 009D5F80
bool __fastcall FableUiCreateBlankTexture(CTexture*,void*,const FableUiDisplayExtent*,int,const int*,unsigned,unsigned,bool,unsigned); // 009FA280
CSurface* __fastcall FableUiGetTextureSurface(CTexture*,void*,CSurface*,unsigned); // 009F9E00
void __fastcall FableUiClearSurface(CSurface*,void*); // 009F40A0
void __fastcall FableUiReleaseSurface(CSurface*,void*); // 009F2E20
