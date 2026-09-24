#include "fable_ui_manager_configuration.h"
void __fastcall FableUiSetInput(FableUiManagerConfigurationView* manager, void*, int type)
{
    manager->InputType=type;
    static const int keys[16]={0,1,3,2,4,5,6,7,8,9,10,11,12,13,14,15};
    static const int keyboard[16]={0x6F,0x70,0x6D,0x72,0x73,0x6E,0x1E,0x30,0x36,0x67,0x10,0x11,0x2C,0x2D,0x18,0x26};
    static const int alternate[4]={0x0F,0x10,0x0D,0x0E};
    unsigned count=type==0 ? 16 : type==1 ? 4 : 0;
    for(unsigned i=0;i<count;++i)
        *FableUiLookupInputKey(&manager->Keys,0,&keys[i])=type==0 ? keyboard[i] : alternate[i];
}
