#include "fable_ui_bank_stream.h"
#include "fable_ui_bank_registry.h"
#include "fable_ui_bank_decode.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "UiBankPathUnavailableServices.h"
void* FableUiBaseVtable=reinterpret_cast<void*>(0x70000100);
FableUiBankRegistryView FableUiBankRegistryState;
unsigned char FableUiGraphicsBankOpenMode;
fable_i32 g_CCharStringInstanceCount_013BD800,g_CWideStringInstanceCount_013BCA20;
FableUiProgressView* FableUiProgressDisplay;
static FableUiBankFileAsyncView bank;
static FableUiRegisteredBank registered;
static FableReferenceCount refs[6];
static unsigned seed,lookupCount;
static FableUiBankTreeNode aliasHead,bankHead;
static FableUiBankHeaderNode bankEntry;
static FableUiRegisteredBankNode registryHead,registryNode;
struct File { void* Vtable; unsigned Id; };
static File files[4];
static unsigned char threadStorage[28];
static CCharStringData inputData,progressData;
static char inputText[]="frontend",progressBuffer[64];
static CWideStringData pathData;
static FableUiBankTreeNode pathHead;
static FableUiBankPathNode pathEntry;
static unsigned char streamBuffer[16384];
static unsigned streamSample;
static unsigned char archiveTemporary[128];
void* __cdecl FableUiAllocateArchiveArray(unsigned size) { printf(" ARALLOC%u",size); return archiveTemporary; }
void __cdecl FableUiFreeArchiveArray(void*) { printf(" ARFREE"); }
void* FableUiDataInputStreamVtable=reinterpret_cast<void*>(0x70000301);
void* FableUiDestroyedBaseVtable=reinterpret_cast<void*>(0x70000302);
FableUiBankStreamVtable FableUiFileInputStreamVtable={0,0,FableUiBankStreamPosition,{0,0,0,0,0},FableUiGetBankStreamSource,FableUiBankStreamUseBuffer,FableUiReadBankStreamDirect};
static FableUiStringValue input;
static unsigned RefId(void* p) { return p ? static_cast<unsigned>(static_cast<FableReferenceCount*>(p)-refs)+1 : 0; }
static unsigned FileId(void* p) { if(!p) return 0; if(p==threadStorage) return 5; return static_cast<File*>(p)->Id; }
static void __fastcall Destroy(void* p) { printf(" DEST%u",FileId(p)); }
static void __fastcall Delete(void* p,void*,unsigned flags) { printf(" DELETE%u:%u",FileId(p),flags); }
static void* threadedVtable[1]={reinterpret_cast<void*>(Delete)};
void* FableUiThreadedFileVtable=threadedVtable;
void* __cdecl FableUiAllocateGraphicsBank(unsigned size)
{
    printf(" ALLOC%u",size);
    if(size==28) return threadStorage;
    return seed&512 ? 0 : &refs[5];
}
void __cdecl FableUiDeleteReference(FableReferenceCount* p)
{ printf(" FREE%u",RefId(p)); }
void* __cdecl FableUiAllocateStringRecord(unsigned size) { printf(" STRALLOC%u",size); return &progressData; }
void __cdecl FableUiFreeStringRecord(void*) { printf(" STRFREE"); }
void* __cdecl FableUiAllocateStringBuffer(unsigned size) { printf(" BUFALLOC%u",size); return size==16384 ? static_cast<void*>(streamBuffer) : static_cast<void*>(progressBuffer); }
void __cdecl FableUiFreeStringBuffer(void* p) { printf(p==streamBuffer ? " STREAMFREE" : " BUFFREE"); }
static void __fastcall Progress(FableUiProgressView*,void*,const FableUiStringValue* name,float amount,bool first,bool second)
{
    printf(" PROGRESS:%s:%g:%u:%u",FableUiStringText(name),amount,first,second);
    if(seed&64) bank.Base.RetailMode=!bank.Base.RetailMode;
    if(((seed>>2)&3)==2) bankHead.Parent=0;
}
static bool __fastcall OpenPath(void*,void*,const FableUiWideStringValue* path,unsigned flags)
{ printf(" OPENPATH%u:%u",path->Storage==&pathData,flags); bank.Base.RetailMode=false; return (seed&16)!=0; }
static unsigned __fastcall GetType(void*,void*) { printf(" TYPE"); return ((seed>>2)&3)==3 ? 43 : 42; }
static void __fastcall FinishRead(void*,void*) { printf(" FINISH"); bank.Base.FileValid=true; }
static FableUiWideStringValue* __fastcall GetPath(void* receiver,void*,FableUiWideStringValue* out)
{ printf(" GETPATH%u",FileId(receiver)); FableUiWideStringValue path={&pathData}; return FableUiCopyWideString(out,0,&path); }
static unsigned __fastcall FileLength(void* p,void*) { printf(" LENGTH%u",FileId(p)); return 0x200000; }
static unsigned __fastcall FilePosition(void* p,void*) { printf(" POSITION%u",FileId(p)); return seed%5; }
static bool __fastcall FileCanSeek(void* p,void*) { printf(" CANSEEK%u",FileId(p)); return (seed&32)!=0; }
static void __fastcall FileSetPosition(void* p,void*,unsigned position) { printf(" SETPOS%u:%u",FileId(p),position); }
static void __fastcall FileRead(void* p,void*,void* target,long size,bool flag)
{
    printf(" FILEREAD%u:%ld:%u",FileId(p),size,flag);
    for(long i=0;i<size;++i) static_cast<unsigned char*>(target)[i]=static_cast<unsigned char>(seed+i*13);
    unsigned length=seed%16; memcpy(static_cast<unsigned char*>(target)+4,&length,4);
    for(unsigned j=0;j<length;++j) static_cast<unsigned char*>(target)[8+j]=static_cast<unsigned char>('A'+j);
}
bool __fastcall FableUiReadBankEntries(FableUiBankFileView* receiver,void*,CFileDataInputStream* stream,unsigned end,unsigned count)
{
    printf(" READ%u:%u:%u:%u",receiver==&bank.Base,FileId(stream->File),end,count);
    FableUiReadBankStreamSlow(stream,0,&streamSample,4); printf(" SAMPLE%08x",streamSample);
    FableUiStringValue name; FableUiReadArchiveString(stream,0,&name);
    printf(" SNAME:%s",FableUiStringText(&name)); FableUiDestroyBankName(&name,0);
    return true;
}
bool __fastcall FableUiOpenThreadedFile(CThreadedFile* file,void*,const FableUiWideStringValue* path,bool nonCached)
{ printf(" THREADOPEN%u:%u:%u",FileId(file),path->Storage==&pathData,nonCached); return (seed&32)!=0; }
static unsigned Normalize(unsigned v)
{
    if(v==reinterpret_cast<unsigned>(bank.Base.Vtable)) return 0x70000200;
    if(v==reinterpret_cast<unsigned>(threadedVtable)) return 0x70000201;
    if(v==reinterpret_cast<unsigned>(&inputData)) return 0x64000000;
    if(v==reinterpret_cast<unsigned>(FableUiDeleteThreadedFile)) return 0x70000202;
    if(v==reinterpret_cast<unsigned>(Destroy)) return 0x70000203;
    if(v==reinterpret_cast<unsigned>(threadStorage)) return 0x62000000;
    for(unsigned i=0;i<4;++i) if(v==reinterpret_cast<unsigned>(&files[i])) return 0x61000000+i*8;
    unsigned begin=reinterpret_cast<unsigned>(refs); if(v>=begin && v<begin+sizeof(refs)) return 0x63000000+v-begin;
    return v;
}
static void Dump(const void* p,unsigned size)
{ for(unsigned i=0;i<size;i+=4) { unsigned v; memcpy(&v,static_cast<const unsigned char*>(p)+i,4); printf(":%08x",Normalize(v)); } }
int main()
{
    FableUiBankOpenVtable table={}; table.OpenPath=OpenPath; table.GetType=GetType; table.FinishRead=FinishRead;
    FableUiFilePathVtable fileTable={}; fileTable.GetPath=GetPath;
    fileTable.Unrecovered00[5]=reinterpret_cast<void*>(FileSetPosition);
    fileTable.Unrecovered00[3]=reinterpret_cast<void*>(FileRead);
    fileTable.Unrecovered00[7]=reinterpret_cast<void*>(FilePosition);
    fileTable.Unrecovered00[9]=reinterpret_cast<void*>(FileLength);
    fileTable.Unrecovered00[10]=reinterpret_cast<void*>(FileCanSeek);
    FableUiProgressVtable progressTable={}; progressTable.Start=Progress; FableUiProgressView progress={&progressTable}; FableUiProgressDisplay=&progress;
    for(unsigned i=0;i<4;++i) { files[i].Vtable=&fileTable; files[i].Id=i+1; }
    for(unsigned mode=0;mode<2;++mode) for(seed=0;seed<1024;++seed)
    {
        memset(&bank,0xA5,sizeof(bank)); memset(threadStorage,0xA5,sizeof(threadStorage)); memset(&progressData,0xA5,sizeof(progressData)); memset(progressBuffer,0xCD,sizeof(progressBuffer));
        bank.Base.Vtable=&table; bank.Base.BankHandle.Storage=0; bank.Base.RetailMode=false; bank.NonCached=(seed&32)!=0;
        memset(refs,0,sizeof(refs)); for(i=0;i<6;++i) { refs[i].owners=i==4 ? 3 : 1; refs[i].destroy=Destroy; refs[i].object=&files[i%4]; }
        bank.Base.BankFile.Data=&files[0]; bank.Base.BankFile.Info=seed&256 ? &refs[0] : 0;
        bank.ThreadedFile.Data=&files[1]; bank.ThreadedFile.Info=seed&256 ? &refs[1] : 0;
        registered.DiskFile.Data=&files[2]; registered.DiskFile.Info=seed&128 ? &refs[2] : 0;
        registered.ThreadedFile.Data=&files[3]; registered.ThreadedFile.Info=seed&128 ? &refs[3] : 0; registered.NonCached=(seed&32)!=0;
        inputData.text=inputText; inputData.unknown04=8; inputData.unknown08=16; inputData.flags0C=0; inputData.owners=1; input.Storage=&inputData;
        g_CCharStringInstanceCount_013BD800=100; g_CWideStringInstanceCount_013BCA20=50; FableUiGraphicsBankOpenMode=seed&1; lookupCount=0;
        memset(&aliasHead,0,sizeof(aliasHead)); memset(&bankHead,0,sizeof(bankHead)); memset(&bankEntry,0,sizeof(bankEntry));
        FableUiBankRegistryState.Aliases.Head=&aliasHead; FableUiBankRegistryState.Files=&registryHead;
        memset(&pathHead,0,sizeof(pathHead)); memset(&pathEntry,0,sizeof(pathEntry));
        pathData.text=L"frontend.big"; pathData.unknown04=reinterpret_cast<unsigned>(pathData.text+12); pathData.unknown08=pathData.unknown04+2; pathData.owners=10;
        pathHead.Parent=&pathEntry.Node.Links; pathEntry.Node.Key=input; pathEntry.Value.Storage=&pathData;
        FableUiBankRegistryState.Paths.Head=&pathHead; FableUiBankRegistryState.BasePath.Storage=0;
        registryHead.Next=registryHead.Previous=&registryNode; registryNode.Next=registryNode.Previous=&registryHead;
        registryNode.Bank.Data=&registered; registryNode.Bank.Info=&refs[4];
        bankEntry.Node.Key=input; bankEntry.Value.Type=42; bankEntry.Value.EntryCount=seed%7 ? 17 : 0xFFFFFFFF;
        bankEntry.Value.Offset=123+seed; bankEntry.Value.Unrecovered0C=0x7654; bankEntry.Value.Alignment=64+seed;
        bankHead.Parent=((seed>>2)&3)==1 ? 0 : &bankEntry.Node.Links; registered.Banks.Head=&bankHead;
        unsigned flags=0xD8|((seed&2) ? 4 : 0);
        printf("TRACE"); bool result=mode ? FableUiOpenAsyncBankReadOnly(&bank,0,&input,flags) : FableUiOpenBankReadOnly(&bank.Base,0,&input,flags);
        printf(" RESULT%u BANK",result); Dump(&bank,sizeof(bank)); printf(" REFS"); Dump(refs,sizeof(refs)); printf(" THREAD"); Dump(threadStorage,sizeof(threadStorage));
        printf(" COUNTS%d:%d:%d:%d",g_CCharStringInstanceCount_013BD800,g_CWideStringInstanceCount_013BCA20,inputData.owners,pathData.owners);
        FableUiReleaseThreadedFile(&bank.ThreadedFile,0); FableUiReleaseDiskFile(&bank.Base.BankFile,0);
        FableUiDeleteThreadedFile(0);
        printf(" END\n");
    }
    return 0;
}
