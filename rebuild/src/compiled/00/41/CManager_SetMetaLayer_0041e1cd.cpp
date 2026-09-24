#include "fable_ui_manager_configuration.h"
void __fastcall FableUiSetMetaLayer(FableUiManagerConfigurationView* manager, void*, int layer)
{
    if(layer<0) layer=0;
    else if(layer>=5) layer=4;
    manager->MetaLayer=layer;
    if(layer==0)
    {
        // Retail skips engine layer 48 hex in this mapping.
        for(int key=-3;key<=13;++key)
            *FableUiLookupLayer(&manager->Layers,0,&key)=0x43+key+(key>=5 ? 1 : 0);
    }
    else
    {
        static const int bases[5]={64,48,32,16,0};
        for(int key=-3;key<=7;++key)
            *FableUiLookupLayer(&manager->Layers,0,&key)=bases[layer]+key+2;
    }
}
