#include "fable_ui_bank_runtime.h"
void __fastcall FableUiInitialiseGraphicsBank(FableUiGraphicsBank* receiver,void*,const FableUiGraphicBankInit* init)
{
    FableUiGraphicsBankRuntimeView* bank=reinterpret_cast<FableUiGraphicsBankRuntimeView*>(receiver);
    bank->UnloadFrameDelay=30; bank->ReduceSizeFrameDelay=3;
    bank->TexturePointersValidForFrames=1; bank->MinAvailablePreloadMemory=0x200000;
    bank->FreezeGraphics=false; bank->ResourceBank.ResourceList.UnloadDelay=3;
    bank->PixelFormats[0]=init->NonAlphaFormat; bank->PixelFormats[1]=init->AlphaFormat;
    bank->PixelFormats[2]=init->BooleanAlphaFormat; bank->PixelFormats[3]=init->InterpolatedAlphaFormat;
    bank->PixelFormats[4]=init->NonAlphaFormat; bank->PixelFormats[5]=init->AlphaFormat;
    bank->PixelFormats[6]=init->UncompressedNonAlphaFormat; bank->PixelFormats[7]=init->UncompressedAlphaFormat;
    bank->PixelFormats[8]=init->UncompressedNonAlphaFormat; bank->PixelFormats[9]=init->UncompressedAlphaFormat;
    bank->PixelFormats[10]=init->SignedFormat;
    bank->TextureSizeWarningMaxWidth=512; bank->TextureSizeWarningMaxHeight=512;
    for(unsigned i=0;i<11;++i)
    {
        int format=bank->PixelFormats[i];
        if(format!=-1)
        {
            FableUiDisplayExtent dimensions={32,32};
            FableUiCreateBlankTexture(&bank->BlankTextures[i],0,&dimensions,-1,&format,0,true,false,0);
            CSurface surface;
            CSurface* returned=FableUiGetTextureSurface(&bank->BlankTextures[i],0,&surface,0);
            FableUiClearSurface(returned,0);
            surface.__vftable=FableUiSurfaceVtable;
            FableUiReleaseSurface(&surface,0);
        }
    }
    bank->AvailableMemory=0; FableUiSetBankPreloadPolicy(bank,0,true); bank->Initialised=true;
}
