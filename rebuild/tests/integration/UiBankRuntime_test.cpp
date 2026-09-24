#include "fable_ui_bank_runtime.h"
#include <stdio.h>
#include <string.h>
void* FableUiBaseVtable=reinterpret_cast<void*>(0x70000100);
void* FableUiResourceBankVtable=reinterpret_cast<void*>(0x70000101);
void* FableUiGraphicResourceBankVtable=reinterpret_cast<void*>(0x70000102);
void* FableUiResourceListVtable=reinterpret_cast<void*>(0x70000103);
void* FableUiResourceVtable=reinterpret_cast<void*>(0x70000104);
void* FableUiGraphicStateVtable=reinterpret_cast<void*>(0x70000105);
void* FableUiSurfaceVtable=reinterpret_cast<void*>(0x70000108);
FableUiGraphicsBankVtable FableUiGraphicsBankRuntimeVtable={};
static FableUiGraphicsBankRuntimeView bank;
static unsigned char managers[2][0x5D4];
static FableReferenceCount references[4];
static unsigned allocationMode,textureMode,referenceCount;
static CSurface alternate;
static unsigned ManagerId(void* p) { return p ? (static_cast<unsigned char*>(p)-managers[0])/0x5D4+1 : 0; }
void* __fastcall FableUiConstructBankFileBase(void* p,void*) { printf(" BASE"); memset(p,0x3C,0x164); return p; }
static void __fastcall Delete(FableUiGraphicsBank* p,void*,unsigned flags) { printf(" DELETE%u:%u",ManagerId(p),flags); }
static FableUiGraphicsBankVtable textureVtable={Delete,0};
void* __cdecl FableUiAllocateGraphicsBank(unsigned size)
{
    printf(" A%u",size);
    if(size==0x5D4) return allocationMode==1 ? 0 : managers[0];
    if(size==12) return allocationMode==2 ? 0 : &references[referenceCount++];
    return 0;
}
void* __fastcall FableUiConstructTextureManager(void* p,void*)
{ printf(" TM%u",ManagerId(p)); *static_cast<FableUiGraphicsBankVtable**>(p)=&textureVtable; return allocationMode==3 ? 0 : p; }
void __cdecl FableUiDeleteReference(FableReferenceCount* r) { printf(" FREE%u",static_cast<unsigned>(r-references)); }
bool __fastcall FableUiCreateBlankTexture(CTexture* texture,void*,const FableUiDisplayExtent* dim,int levels,const int* format,unsigned usage,unsigned managed,bool dynamic,unsigned extra)
{
    unsigned i=static_cast<unsigned>(texture-bank.BlankTextures);
    printf(" TEX%u:%d:%d:%d:%d:%u:%u:%u:%u",i,dim->Width,dim->Height,levels,*format,usage,managed,dynamic,extra);
    unsigned words[2]={0x81000000+i*4,0x10000000+i}; memcpy(texture,words,8);
    if(textureMode==3 && i+1<11) bank.PixelFormats[i+1]=-1;
    return textureMode!=1;
}
CSurface* __fastcall FableUiGetTextureSurface(CTexture* texture,void*,CSurface* out,unsigned level)
{
    unsigned i=static_cast<unsigned>(texture-bank.BlankTextures); printf(" SURF%u:%u",i,level);
    out->__vftable=reinterpret_cast<void*>(0x12345678); out->PD3DSurface=reinterpret_cast<IDirect3DSurface9*>(i+1); out->AllocationSource=0x1234; out->PAllocatedMemory=reinterpret_cast<void*>(0x5678);
    alternate=*out; alternate.PD3DSurface=reinterpret_cast<IDirect3DSurface9*>(i+101);
    return textureMode==2 ? &alternate : out;
}
void __fastcall FableUiClearSurface(CSurface* s,void*) { printf(" CLEAR%u",reinterpret_cast<unsigned>(s->PD3DSurface)); }
void __fastcall FableUiReleaseSurface(CSurface* s,void*) { printf(" RELEASE%u:%u:%u:%u",s->__vftable==FableUiSurfaceVtable,reinterpret_cast<unsigned>(s->PD3DSurface),s->AllocationSource,reinterpret_cast<unsigned>(s->PAllocatedMemory)); }
static unsigned Normalize(unsigned value)
{
    if(value==reinterpret_cast<unsigned>(&FableUiGraphicsBankRuntimeVtable)) return 0x70000106;
    if(value==reinterpret_cast<unsigned>(FableUiDeleteTextureManager)) return 0x70000107;
    unsigned base=reinterpret_cast<unsigned>(&bank);
    if(value>=base && value<base+sizeof(bank)) return 0x60000000+value-base;
    base=reinterpret_cast<unsigned>(managers);
    if(value>=base && value<base+sizeof(managers)) return 0x61000000+value-base;
    base=reinterpret_cast<unsigned>(references);
    if(value>=base && value<base+sizeof(references)) return 0x62000000+value-base;
    return value;
}
static void Dump()
{
    printf(" BANK");
    for(unsigned i=0;i<sizeof(bank);i+=4) { unsigned word; memcpy(&word,reinterpret_cast<unsigned char*>(&bank)+i,4); printf(":%08x",Normalize(word)); }
    for(unsigned r=0;r<referenceCount;++r) printf(" REF%u:%d:%08x:%08x",r,references[r].owners,Normalize(reinterpret_cast<unsigned>(references[r].destroy)),Normalize(reinterpret_cast<unsigned>(references[r].object)));
}
int main(int argc,char** argv)
{
    if(argc!=2) return 2; FILE* file=fopen(argv[1],"r"); if(!file) return 3;
    unsigned mask,pattern;
    while(fscanf(file,"%u %u %u %u",&mask,&pattern,&allocationMode,&textureMode)==4)
    {
        memset(&bank,pattern,sizeof(bank)); memset(references,0xA5,sizeof(references)); referenceCount=0;
        printf("TRACE");
        if(FableUiConstructGraphicsBank(&bank,0)!=reinterpret_cast<FableUiGraphicsBank*>(&bank)) return 4;
        Dump();
        FableUiGraphicBankInit init; memset(&init,0xA5,sizeof(init));
        for(unsigned i=0;i<7;++i) reinterpret_cast<int*>(&init)[i]=(mask&(1u<<i)) ? static_cast<int>(i*17) : -1;
        FableUiInitialiseGraphicsBank(reinterpret_cast<FableUiGraphicsBank*>(&bank),0,&init); Dump();
        *reinterpret_cast<FableUiGraphicsBankVtable**>(managers[1])=&textureVtable;
        FableUiResetTextureManager(&bank.TextureManager,0,managers[1]); Dump();
        FableUiResetTextureManager(&bank.TextureManager,0,0); FableUiDeleteTextureManager(0); Dump();
        printf(" END\n");
    }
    fclose(file); return 0;
}
