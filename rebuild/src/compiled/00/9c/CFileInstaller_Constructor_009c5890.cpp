#include "fable_file_installer.h"
#include "fable_retail_banks.h"
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(void*);
extern "C" __declspec(dllimport) void* __stdcall CreateEventW(void*,int,int,const wchar_t*);
extern "C" unsigned long __cdecl _beginthread(void(__cdecl*)(void*),unsigned int,void*);
extern const char g_FileInstallerCompressedExtension_0129B448[];
extern const char g_FileInstallerPlainExtension_0129B440[];
void* FABLE_CDECL FableInstallerContainerAllocate(unsigned long);
void FABLE_CDECL FableInstallerWorker_009C5870(void*);
void FABLE_CDECL FableInstallerDecompressionWorker_009C4420(void*);
static __forceinline void InitialiseList(void*& head,unsigned long bytes){head=0;void* node=FableInstallerContainerAllocate(bytes);((void**)node)[0]=node;((void**)node)[1]=node;head=node;}
static __forceinline void InitialiseTree(void*& head,unsigned long& count,unsigned long bytes){head=0;head=FableInstallerContainerAllocate(bytes);count=0;*(unsigned char*)head=0;((unsigned long*)head)[1]=0;((void**)head)[2]=head;((void**)head)[3]=head;}
CFileInstaller::CFileInstaller()
{
    unsigned char* bytes=reinterpret_cast<unsigned char*>(this);
    unsigned long* fields=reinterpret_cast<unsigned long*>(this);
    fields[7]=0;fields[8]=0;fields[9]=0;
    InitialiseList(*reinterpret_cast<void**>(bytes+0x38),16);
    InitialiseList(*reinterpret_cast<void**>(bytes+0x3c),16);
    InitialiseList(*reinterpret_cast<void**>(bytes+0x40),184);
    InitialiseTree(*reinterpret_cast<void**>(bytes+0x44),fields[0x48/4],20);
    InitialiseTree(*reinterpret_cast<void**>(bytes+0x94),fields[0x98/4],24);
    InitializeCriticalSection(bytes+4);
    typedef void*(__stdcall* EventCreator)(void*,int,int,const wchar_t*);
    EventCreator create=CreateEventW;
    bytes[0x79]=0;bytes[0x78]=0;bytes[0x7a]=0;bytes[0x7b]=0;bytes[0x80]=1;fields[0x7c/4]=0;
    fields[0x84/4]=reinterpret_cast<unsigned long>(create(0,0,0,0));
    fields[0x88/4]=reinterpret_cast<unsigned long>(create(0,0,0,0));
    fields[0x8c/4]=reinterpret_cast<unsigned long>(create(0,0,0,0));
    fields[0x90/4]=reinterpret_cast<unsigned long>(create(0,0,0,0));
    fields[0x74/4]=0;bytes[0x78]=0;
    fields[0x50/4]=0;fields[0x54/4]=0;fields[0x28/4]=0;fields[0x2c/4]=0;fields[0x64/4]=0;fields[0x68/4]=0;fields[0x30/4]=0;fields[0x34/4]=0;fields[0x6c/4]=0;fields[0x70/4]=0;
    bytes[0x7a]=0;fields[0x58/4]=0;fields[0x5c/4]=0;bytes[0x60]=0;bytes[0x81]=0;
    {CCharString compressed(g_FileInstallerCompressedExtension_0129B448,-1);(*reinterpret_cast<CBasicRetailBankAliasMap*>(bytes+0x94))[compressed]=g_FileInstallerPlainExtension_0129B440;}
    _beginthread(FableInstallerWorker_009C5870,0x3000,this);
    _beginthread(FableInstallerDecompressionWorker_009C4420,0x3000,this);
}
