#include "fable_ui_display_services.h"
#include <stdio.h>
#include <string.h>
unsigned char FableUiCoordinateConversionEnabled;
FableUiStateVector2 FableUiCoordinateSourceExtent={1280,720};
FableUiStateVector2 FableUiCoordinateDestinationExtent;
void* FableUiScaleContext;
static FableUiSystemManagerView systemManager;
static FableUiDisplayExtent extent;
FableUiSystemManagerView* __cdecl FableUiGetSystemManager() { printf(" S"); return &systemManager; }
void __fastcall FableUiQueryDisplayExtent(void* display,void*,FableUiDisplayExtent* out)
{
    if(display!=&extent) { printf(" BADDISPLAY"); return; }
    printf(" D%u",FableUiCoordinateConversionEnabled); *out=extent;
}
static unsigned Bits(float value) { unsigned out; memcpy(&out,&value,4); return out; }
int main(int argc,char** argv)
{
    if(argc!=2) return 2; FILE* f=fopen(argv[1],"r"); if(!f) return 3;
    unsigned initial,sequence,context,seed; int width,height;
    while(fscanf(f,"%u %u %u %u %d %d",&initial,&sequence,&context,&seed,&width,&height)==6)
    {
        FableUiCoordinateConversionEnabled=static_cast<unsigned char>(initial);
        FableUiCoordinateDestinationExtent.x=seed ? -1.0f : 1920.0f; FableUiCoordinateDestinationExtent.y=seed ? 0.0f : 1080.0f;
        FableUiScaleContext=context ? &extent : 0; systemManager.Display=&extent; printf("TRACE");
        for(unsigned i=0;i<3;++i)
        {
            extent.Width=static_cast<int>(static_cast<unsigned>(width)+i); extent.Height=static_cast<int>(static_cast<unsigned>(height)+i);
            FableUiSetRelativeCoordinates((sequence&(1<<i))!=0);
            FableUiStateVector2 scale; FableUiGetManagerScale(0,0,&scale);
            printf(" R%u:%08x:%08x:%08x:%08x",FableUiCoordinateConversionEnabled,Bits(FableUiCoordinateDestinationExtent.x),Bits(FableUiCoordinateDestinationExtent.y),Bits(scale.x),Bits(scale.y));
        }
        printf(" END\n");
    }
    fclose(f); return 0;
}
