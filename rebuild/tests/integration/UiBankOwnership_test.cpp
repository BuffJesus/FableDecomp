#include "fable_ui_bank_ownership.h"
#include <stdio.h>
#include <string.h>

static FableUiManagerView manager;
static FableReferenceCount infos[3];
static unsigned mutation;
static FableUiBankReference* Owned()
{ return reinterpret_cast<FableUiBankReference*>(&manager.Configuration.GraphicsBank); }
static unsigned Id(void* p)
{ return !p ? 0 : static_cast<unsigned>(static_cast<FableReferenceCount*>(p)-infos)+1; }
static void Snapshot()
{ printf(":%08x:%u:%d:%d:%d",Owned()->Data,Id(Owned()->Info),infos[0].owners,infos[1].owners,infos[2].owners); }
static void __fastcall Destroy(void* object)
{
    printf(" D%u",reinterpret_cast<unsigned>(object)); Snapshot();
    if(mutation) Owned()->Info=&infos[2];
}
void __cdecl FableUiDeleteReference(FableReferenceCount* info)
{ printf(" F%u",Id(info)); Snapshot(); }
int main(int argc,char** argv)
{
    if(argc!=2) return 2; FILE* input=fopen(argv[1],"r"); if(!input) return 3;
    unsigned mode,oldId,newId,oldCount,newCount,sameData,repeat;
    while(fscanf(input,"%u %u %u %u %u %u %u %u",&mode,&oldId,&newId,&oldCount,&newCount,&sameData,&repeat,&mutation)==8)
    {
        memset(&manager,0xA5,sizeof(manager));
        for(unsigned i=0;i<3;++i) { infos[i].owners=7; infos[i].destroy=Destroy; infos[i].object=reinterpret_cast<void*>(i+1); }
        if(newId) infos[newId-1].owners=newCount;
        if(oldId) infos[oldId-1].owners=oldCount;
        Owned()->Data=reinterpret_cast<void*>(0x11111111); Owned()->Info=oldId ? &infos[oldId-1] : 0;
        FableUiBankReference incoming={reinterpret_cast<void*>(sameData==2 ? 0 : sameData ? 0x11111111 : 0x22222222),newId ? &infos[newId-1] : 0};
        printf("TRACE");
        for(unsigned step=0;step<repeat;++step)
        {
            // Model the caller acquiring a fresh by-value parameter each time.
            if(mode==2 && incoming.Info) ++static_cast<FableReferenceCount*>(incoming.Info)->owners;
            if(mode==0) FableUiReleaseBankReference(Owned(),0);
            else if(mode==1) FableUiShareBankReference(Owned(),0,incoming.Data,incoming.Info);
            else FableUiSetGraphicsBank(&manager,0,incoming);
            printf(" S"); Snapshot();
        }
        FableUiReleaseBankReference(Owned(),0); printf(" Z"); Snapshot();
        unsigned char* bytes=reinterpret_cast<unsigned char*>(&manager);
        for(unsigned j=0;j<sizeof(manager);++j) if((j<0x10 || j>=0x18) && bytes[j]!=0xA5) return 4;
        printf(" END\n");
    }
    fclose(input); return 0;
}
