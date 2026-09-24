#include "fable_ui_bank_factory.h"
#include <stdio.h>
#include <string.h>

unsigned char FableUiGraphicsBankOpenMode;
FableUiProgressView* FableUiProgressDisplay;
fable_i32 g_CCharStringInstanceCount_013BD800;
static unsigned char strings[16][32],buffers[16][128],banks[8][0x30C];
static FableReferenceCount references[8];
static FableUiGraphicsBankNode nodes[8],head;
static FableUiGraphicsBankManagerView manager;
static unsigned ns,nb,ng,nr,nn,infoAttempts,failInfo;
static unsigned Id(const void* p,const void* base,unsigned stride)
{ return p ? (static_cast<const unsigned char*>(p)-static_cast<const unsigned char*>(base))/stride+1 : 0; }
static unsigned NodeId(FableUiGraphicsBankNode* n) { return n==&head ? 0 : Id(n,nodes,sizeof(nodes[0])); }
static void Hex(const void* p,unsigned n)
{ for(unsigned i=0;i<n;++i) printf("%02x",static_cast<const unsigned char*>(p)[i]); }
static void Text(const FableUiStringValue* s)
{ const char* p=FableUiStringText(s); Hex(p,static_cast<unsigned>(strlen(p))); }
void* __cdecl FableUiAllocateStringRecord(unsigned n) { printf(" SR%u",n); return strings[ns++]; }
void __cdecl FableUiFreeStringRecord(void* p) { printf(" SF%u",Id(p,strings,32)); }
void* __cdecl FableUiAllocateStringBuffer(unsigned n) { printf(" SB%u",n); return buffers[nb++]; }
void __cdecl FableUiFreeStringBuffer(void* p) { printf(" ST%u",Id(p,buffers,128)); }
void* __cdecl FableUiAllocateGraphicsBank(unsigned size)
{
    printf(" A%u",size);
    if(size==0x30C) return banks[ng++];
    if(size==12)
    { ++infoAttempts; if(failInfo==2 || (failInfo==1 && infoAttempts==1)) { printf(" NOINFO"); return 0; } return &references[nr++]; }
    return 0;
}
void* __cdecl FableUiAllocateGraphicsBankNode(unsigned size) { printf(" L%u",size); return &nodes[nn++]; }
static void __fastcall Delete(FableUiGraphicsBank* bank,void*,unsigned flags)
{ printf(" DELETE%u:%u",Id(bank,banks,0x30C),flags); }
static void __fastcall Open(FableUiGraphicsBank* bank,void*,const FableUiStringValue* name,unsigned flags)
{ printf(" OPEN%u:%u:",Id(bank,banks,0x30C),flags); Text(name); }
static FableUiGraphicsBankVtable bankVtable={Delete,Open};
FableUiGraphicsBank* __fastcall FableUiConstructGraphicsBank(void* p,void*)
{ printf(" CTOR%u",Id(p,banks,0x30C)); FableUiGraphicsBank* b=static_cast<FableUiGraphicsBank*>(p); b->Vtable=&bankVtable; return b; }
void __fastcall FableUiInitialiseGraphicsBank(FableUiGraphicsBank* b,void*,const FableUiGraphicBankInit* init)
{ printf(" INIT%u:",Id(b,banks,0x30C)); Hex(init,44); }
void __cdecl FableUiDeleteReference(FableReferenceCount* info) { printf(" RF%u",Id(info,references,12)); }
static void __fastcall Start(FableUiProgressView*,void*,const FableUiStringValue* name,float amount,bool flag,bool last)
{
    unsigned bits; memcpy(&bits,&amount,4); printf(" PROGRESS%08x:%u:%u:",bits,flag,last); Text(name);
    FableUiGraphicsBankOpenMode^=1;
}
static void Snapshot(FableUiBankReference* results,unsigned count)
{
    printf(" S%u:%d:%u",FableUiGraphicsBankOpenMode,g_CCharStringInstanceCount_013BD800,ng);
    for(unsigned i=0;i<count;++i) printf(" O%u:%u",Id(results[i].Data,banks,0x30C),Id(results[i].Info,references,12));
    for(unsigned j=0;j<nr;++j) printf(" R%u:%d:%u:%u",j,references[j].owners,references[j].destroy==FableUiDestroyGraphicsBank,Id(references[j].object,banks,0x30C));
    printf(" HEAD%u:%u",NodeId(head.Next),NodeId(head.Previous));
    for(unsigned k=0;k<nn;++k)
    {
        FableUiGraphicsBankNode& n=nodes[k];
        printf(" N%u:%u:%u:%u:%u:%u:",k,NodeId(n.Next),NodeId(n.Previous),Id(n.Name.Storage,strings,32),Id(n.Bank.Data,banks,0x30C),Id(n.Bank.Info,references,12)); Hex(&n.Init,44);
    }
    for(unsigned s=0;s<ns;++s) { CCharStringData* d=reinterpret_cast<CCharStringData*>(strings[s]); printf(" T%u:%d",s,d->owners); }
}
int main(int argc,char** argv)
{
    if(argc!=2) return 2; FILE* file=fopen(argv[1],"r"); if(!file) return 3;
    unsigned nameCase,variant,openMode,progress,pattern;
    FableUiProgressVtable pv={}; pv.Start=Start; FableUiProgressView p={&pv};
    while(fscanf(file,"%u %u %u %u %u %u",&nameCase,&variant,&openMode,&failInfo,&progress,&pattern)==6)
    {
        ns=nb=ng=nr=nn=infoAttempts=0; memset(strings,0xA5,sizeof(strings)); memset(buffers,0xCD,sizeof(buffers)); memset(nodes,0xA5,sizeof(nodes)); memset(banks,0xA5,sizeof(banks)); memset(references,0xA5,sizeof(references));
        head.Next=head.Previous=&head; manager.Banks=&head; FableUiGraphicsBankOpenMode=static_cast<unsigned char>(openMode); FableUiProgressDisplay=progress ? &p : 0; g_CCharStringInstanceCount_013BD800=500;
        char a[32]="GBANK_MAIN",b[32];
        if(nameCase==1) a[0]=0;
        if(nameCase==2) { a[0]='X'; a[1]=static_cast<char>(0x80); a[2]='a'; a[3]=0; }
        if(nameCase==3) strcpy(a,"AB");
        memcpy(b,a,32);
        if(variant==2) b[0]=b[0] ? 'z' : 'Q';
        FableUiStringValue names[2]; FableUiBankReference results[3]; FableUiGraphicBankInit init;
        memset(&init,pattern,sizeof(init));
        printf("TRACE"); FableUiConstructBankName(&names[0],0,a,-1);
        FableUiConstructBankName(&names[1],0,b,variant==3 ? static_cast<long>(strlen(b)+2) : -1);
        for(unsigned call=0;call<3;++call)
        {
            const FableUiStringValue* name=call==1 && variant!=0 ? &names[1] : &names[0];
            if(FableUiCreateGraphicsBank(&manager,0,&results[call],name,&init,(call&1)!=0,(call&2)!=0)!=&results[call]) return 4;
            Snapshot(results,call+1); init.AlphaFormat=static_cast<int>(call+123);
        }
        printf(" CLEAN");
        for(unsigned i=0;i<3;++i) FableUiReleaseBankReference(&results[i],0);
        for(unsigned j=0;j<nn;++j) { FableUiReleaseBankReference(&nodes[j].Bank,0); FableUiDestroyBankName(&nodes[j].Name,0); }
        FableUiDestroyBankName(&names[0],0); FableUiDestroyBankName(&names[1],0);
        Snapshot(results,3); printf(" END\n");
    }
    fclose(file); return 0;
}
