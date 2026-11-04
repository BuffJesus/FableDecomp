#include "fable_file_installer_active_files.h"
#include "fable_retail_banks.h"
#include "fable_critical_section_lock.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char* const region=(unsigned char*)0x2004000;
static FILE* input;static unsigned args[4],position,sleeps,signals,closes;
static unsigned char snapshot[0x8000],actual[0x8000],after[0x8000];
static void* derivedVtable;static void* baseVtable;
static void read(void* p,unsigned size){if(fread(p,1,size,input)!=size)abort();}
static void canonical(){memcpy(actual,region,sizeof(actual));void* v=*(void**)actual;if(v!=derivedVtable&&v!=baseVtable)abort();*(unsigned*)actual=v==baseVtable?0x129b414:0x129b43c;}
static void boundary(unsigned kind,unsigned a=0,unsigned b=0){
 unsigned row[3];read(row,sizeof(row));read(snapshot,sizeof(snapshot));
 if(!position)derivedVtable=*(void**)region;
 canonical();if(row[0]!=kind||row[1]!=a||row[2]!=b||memcmp(actual,snapshot,sizeof(actual))){printf("boundary=%u kind=%u expected=%u\n",position,kind,row[0]);exit(9);}++position;
}
CCriticalSectionLock::CCriticalSectionLock(_RTL_CRITICAL_SECTION& cs,bool only){boundary(1,(unsigned)&cs,only);}
void CCriticalSectionLock::Release(){boundary(4);}
void CCriticalSectionLock::Lock(){boundary(6);}
CCriticalSectionLock::~CCriticalSectionLock(){boundary(8);}
void CFileInstaller::ClearBuffers(){boundary(2,(unsigned)this);for(unsigned off=0x1c;off<=0x2c;off+=4)*(unsigned*)(region+off)=0;}
static int __stdcall Signal(void* p){boundary(3,(unsigned)p);if(args[3]==1&&!signals)*(unsigned*)(region+0x88)=0x9999;++signals;return 1;}
static void __stdcall Yield(unsigned n){boundary(5,n);if(++sleeps==args[0])region[0x78]=region[0x79]=1;}
static int __stdcall Close(void* p){boundary(7,(unsigned)p);if(args[3]==1&&!closes)*(unsigned*)(region+0x88)=0xaaaa;++closes;return 1;}
static void __stdcall DeleteCS(void* p){boundary(9,(unsigned)p);}
extern "C" {int(__stdcall* SignalSlot)(void*)=Signal;void(__stdcall* SleepSlot)(unsigned)=Yield;int(__stdcall* CloseSlot)(void*)=Close;void(__stdcall* DeleteSlot)(void*)=DeleteCS;}
void CBasicRetailBankAliasMap::Erase(CNode* n){boundary(10,(unsigned)this,(unsigned)n);if(args[3]==2)*(unsigned*)(region+0x94)=0x200b000;}
void InstallerCompletedFileSet::Erase(Node* n){boundary(11,(unsigned)this,(unsigned)n);}
void InstallerActiveFileList::Clear(){boundary(12,(unsigned)this);head->next=head;head->previous=head;}
void InstallerCountedFileList::Clear(){boundary(13,(unsigned)this);head->next=head;head->previous=head;}
void FABLE_FASTCALL FableInstallerDestroyBufferRange(InstallerBufferChunk* first,InstallerBufferChunk* last,void*){boundary(14,(unsigned)first,(unsigned)last);if(args[3]==2)*(unsigned*)(region+0x1c)=0x200b100;}
extern "C" void __cdecl Fnod(void* p){boundary(15,(unsigned)p);}
int main(int argc,char** argv){
 if(argc!=2)return 2;input=fopen(argv[1],"rb");if(!input)return 2;
 if(VirtualAlloc((void*)0x2000000,0x20000,0x3000,4)!=(void*)0x2000000)return 3;
 unsigned long baseStorage;CTBaseSingleton<CFileInstaller>* base=new(&baseStorage)CTBaseSingleton<CFileInstaller>;baseVtable=*(void**)base;
 unsigned cases;read(&cases,4);
 for(unsigned i=0;i<cases;++i){unsigned events;read(args,sizeof(args));read(&events,4);read(region,sizeof(actual));read(after,sizeof(after));position=sleeps=signals=closes=0;derivedVtable=0;
  reinterpret_cast<CFileInstaller*>(region)->CFileInstaller::~CFileInstaller();canonical();
  if(position!=events||memcmp(actual,after,sizeof(actual)))return 4;
 }
 if(fgetc(input)!=EOF)return 5;printf("INSTALLER_DESTRUCTOR_NATIVE_OK cases=%u\n",cases);return 0;
}
