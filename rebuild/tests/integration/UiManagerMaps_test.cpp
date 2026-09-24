#include "fable_ui_manager_configuration.h"
#include <stdio.h>
#include <string.h>
static FableUiManagerConfigurationView manager;
static FableUiIntegerMapNode pool[128];
static unsigned allocated;
static bool tracing;
void* FableUiAllocateIntegerMapNode(unsigned size)
{
    if(allocated>=128) return 0;
    if(tracing) printf(" A%u:%u",allocated,size);
    return pool+allocated++;
}
static int Id(FableUiEventTreeNode* p)
{ if(!p) return -1; for(unsigned i=0;i<allocated;++i) if(p==&pool[i].LinksAndKey) return i; return -2; }
int main(int argc,char** argv)
{
    if(argc!=2) return 2; FILE* f=fopen(argv[1],"r"); if(!f) return 3;
    unsigned op,seed; int a,b,c;
    while(fscanf(f,"%u %u %d %d %d",&op,&seed,&a,&b,&c)==5)
    {
        memset(&manager,0xA5,sizeof(manager)); memset(pool,0xCD,sizeof(pool)); allocated=2; tracing=false;
        manager.Keys.Head=&pool[0].LinksAndKey; manager.Layers.Head=&pool[1].LinksAndKey;
        manager.Keys.Count=manager.Layers.Count=0;
        for(unsigned i=0;i<2;++i) { pool[i].LinksAndKey.Colour=0; pool[i].LinksAndKey.Parent=0; pool[i].LinksAndKey.Left=pool[i].LinksAndKey.Right=&pool[i].LinksAndKey; }
        // Seed alternating keys, leaving missing entries for the recovered path.
        for(int i=0;i<24;i+=2)
        { int key=i-3; *FableUiLookupInputKey(&manager.Keys,0,&key)=static_cast<int>(seed+i); *FableUiLookupLayer(&manager.Layers,0,&key)=static_cast<int>(seed-i); }
        tracing=true; printf("TRACE"); int values[3]={a,b,c};
        for(unsigned i=0;i<3;++i)
            if(op) FableUiSetMetaLayer(&manager,0,values[i]); else FableUiSetInput(&manager,0,values[i]);
        printf(" I%d M%d K%u L%u",manager.InputType,manager.MetaLayer,manager.Keys.Count,manager.Layers.Count);
        for(unsigned i=0;i<allocated;++i)
        { FableUiIntegerMapNode& n=pool[i]; printf(" T%u:%u:%d:%d:%d:%d:%d",i,n.LinksAndKey.Colour,Id(n.LinksAndKey.Parent),Id(n.LinksAndKey.Left),Id(n.LinksAndKey.Right),n.LinksAndKey.Event,n.Value); }
        printf(" END\n");
    }
    fclose(f); return 0;
}
