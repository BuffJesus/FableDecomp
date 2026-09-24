#include "fable_ui_display_formats.h"
#include "fable_ui_manager_construction.h"
#include <stdio.h>
#include <string.h>
FableUiPixelFormatDescription FableUiPixelFormats[64];
FableUiSystemManagerView FableUiSystemManagerState;
static FableUiDisplayFormatView display;
static unsigned policy;
static long __stdcall Check(FableUiD3D9*,unsigned adapter,unsigned device,unsigned mode,unsigned usage,unsigned resource,unsigned format)
{
    unsigned choice=(format^policy)%3;
    long result=policy==0 ? 0 : policy==1 ? -1 : choice==0 ? static_cast<long>(0x80000000u) : choice==1 ? 1 : 0;
    printf(" Q%u:%u:%u:%u:%u:%u:%ld",adapter,device,mode,usage,resource,format,result);
    if(policy&8) { ++display.AdapterOrdinal; ++display.DeviceType; ++display.CurrentModeFormat; }
    return result;
}
int main(int argc,char** argv)
{
    if(argc!=2) return 2; FILE* file=fopen(argv[1],"r"); if(!file) return 3;
    FableUiD3D9Vtable vtable={}; vtable.CheckDeviceFormat=Check; FableUiD3D9 d3d={&vtable};
    unsigned mode,usage,rowCount; int bits,width,height;
    while(fscanf(file,"%u %d %u %u %d %d %u",&mode,&bits,&policy,&usage,&width,&height,&rowCount)==7)
    {
        if(rowCount>62) return 4;
        memset(FableUiPixelFormats,0,sizeof(FableUiPixelFormats));
        for(unsigned i=0;i<64;++i) FableUiPixelFormats[i].Bits=-1;
        for(unsigned r=0;r<rowCount;++r)
        {
            FableUiPixelFormatDescription& p=FableUiPixelFormats[r];
            if(fscanf(file,"%u %d %d %d %d %d %d %d %d",&p.D3DFormat,&p.Type,&p.Bits,&p.Alpha,&p.Red,&p.Green,&p.Blue,&p.Stencil,&p.FloatingPoint)!=9) return 5;
        }
        memset(&display,0xA5,sizeof(display)); display.D3D=&d3d; display.AdapterOrdinal=7; display.DeviceType=2; display.CurrentModeFormat=21;
        display.RenderTargetDimensions.Width=width; display.RenderTargetDimensions.Height=height;
        int out=-1234567; bool ok=false; printf("TRACE");
        switch(mode)
        {
        case 0: ok=FableUiChooseNonAlphaFormat(&display,0,bits,&out); break;
        case 1: ok=FableUiChooseAlphaFormat(&display,0,bits,&out); break;
        case 2: ok=FableUiChooseBooleanAlphaFormat(&display,0,bits,&out); break;
        case 3: ok=FableUiChooseUncompressedAlphaFormat(&display,0,bits,&out); break;
        case 4: ok=FableUiChooseUncompressedNonAlphaFormat(&display,0,bits,&out,usage!=0); break;
        case 5: ok=FableUiChooseSignedFormat(&display,0,bits,&out); break;
        case 6: FableUiSetPixelFormat(&out,0,static_cast<unsigned>(bits)); break;
        default: return 6;
        }
        FableUiDisplayExtent extent; FableUiQueryDisplayExtent(&display,0,&extent);
        printf(" O%u:%d D%d:%d G%u C%u:%u:%u END\n",ok,out,extent.Width,extent.Height,FableUiGetSystemManager()==&FableUiSystemManagerState,display.AdapterOrdinal,display.DeviceType,display.CurrentModeFormat);
    }
    fclose(file); return 0;
}
