#include "fable_ui_manager_configuration.h"
#include <stdio.h>
#include <string.h>
static FableUiManagerConfigurationView manager;
static int keys[24],layers[24];
int* __fastcall FableUiLookupInputKey(FableUiIntegerMapStorage* map,void*,const int* key)
{
    if(map!=&manager.Keys) return 0;
    printf(" K%d:%d",*key,manager.InputType); return keys+*key+3;
}
int* __fastcall FableUiLookupLayer(FableUiIntegerMapStorage* map,void*,const int* key)
{
    if(map!=&manager.Layers) return 0;
    printf(" L%d:%d",*key,manager.MetaLayer); return layers+*key+3;
}
int main(int argc,char** argv)
{
    if(argc!=2) return 2; FILE* f=fopen(argv[1],"r"); if(!f) return 3;
    unsigned op,seed; int a,b,c;
    while(fscanf(f,"%u %u %d %d %d",&op,&seed,&a,&b,&c)==5)
    {
        memset(&manager,0xA5,sizeof(manager));
        for(unsigned i=0;i<24;++i) { keys[i]=static_cast<int>(seed+i); layers[i]=static_cast<int>(seed-i); }
        int values[3]={a,b,c}; printf("TRACE");
        for(unsigned i=0;i<3;++i)
            if(op) FableUiSetMetaLayer(&manager,0,values[i]); else FableUiSetInput(&manager,0,values[i]);
        printf(" I%d M%d K",manager.InputType,manager.MetaLayer);
        for(unsigned i=0;i<24;++i) printf(" %d",keys[i]);
        printf(" L"); for(unsigned i=0;i<24;++i) printf(" %d",layers[i]); printf(" END\n");
    }
    fclose(f); return 0;
}
