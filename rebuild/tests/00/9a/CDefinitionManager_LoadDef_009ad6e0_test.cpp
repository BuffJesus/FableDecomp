#include "fable_definition_manager.h"
#include "fable_string_parser.h"
#include "fable_persist_context.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char g_FableEmptyNarrowString_0129AAF4[1]={0};
static CCharStringData*& slot(const CCharString& s){return *reinterpret_cast<CCharStringData**>(const_cast<CCharString*>(&s));}
static const char* text(const CCharString& s){return slot(s)?slot(s)->Data.PData:"";}
static CCharStringData* make(const char* s,long length){if(length<0)length=strlen(s);CCharStringData* d=(CCharStringData*)calloc(1,sizeof(CCharStringData));d->Data.PData=(char*)malloc(length+1);memcpy(d->Data.PData,s,length);d->Data.PData[length]=0;d->Data.StringLength=length;return d;}
static void release(CCharStringData* d){if(d){free(d->Data.PData);free(d);}}
CCharString::CCharString():storage_(0){}
CCharString::CCharString(const char* s,long length):storage_(make(s,length)){}
CCharString::CCharString(const CCharString& s):storage_(make(text(s),-1)){}
CCharString::~CCharString(){release(storage_);}
CCharString::operator const char*()const{return text(*this);}
CCharString& CCharString::operator=(const CCharString& s){CCharStringData* d=make(text(s),-1);release(storage_);storage_=d;return *this;}
const CCharString& CCharString::operator=(const char* s){CCharStringData* d=make(s,-1);release(storage_);storage_=d;return *this;}
void CCharString::SetAsSubString(const char* s,long length){CCharStringData* d=make(s,length);release(storage_);storage_=d;}
template<>bool CBasicString<char>::operator==(const char* s)const{return !strcmp(PData,s);}
CCharString FABLE_FASTCALL operator+(const CCharString& a,const CCharString& b){char s[256];strcpy(s,text(a));strcat(s,text(b));return CCharString(s);}
CCharString FABLE_FASTCALL operator+(const CCharString& a,const char* b){char s[256];strcpy(s,text(a));strcat(s,b);return CCharString(s);}

#include "fable_def_class_base.h"
#include "fable_data_input_stream.h"
#include "fable_progress_display.h"
#include "test_support/data_input_stream_stubs.h"
static unsigned test,destructs;static bool good;static char events[64];static unsigned used;static CDefinitionManager* manager;static CDefClassBase* definition;static CDefClassBase* old;static CDefinitionManager::CDefClassInfo* classInfo;static CDefinitionManager::CInstantiatedDefInfo* entry;
static void log(char ch){events[used++]=ch;events[used]=0;}
CBaseClass::CBaseClass(){}
CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){++destructs;}
CDataInputStream::CDataInputStream():StreamPos(0),StreamSize(0),CurrentSourcePtr(0),SourceChunkStreamPos(0),SourceChunkBytesRemaining(0){}
void CDataInputStream::SetPosition(unsigned long){abort();}
CDefString::operator CCharString(){good &= TablePos==100;return CCharString("CLASS");}
void FABLE_FASTCALL NProgressDisplay::DisplayProgress(const CCharString& value,float fraction,bool render,bool debug){good &= fraction==-1.0f&&!render&&!debug;const char* message=text(value);log(message[strlen(message)-1]);}
void CDefinitionManager::DecompressDefinitionData(CMemoryDataInputStream& stream,unsigned long index)const{good &= index==1;stream.StreamSize=100;log('D');if(test==5)entry->PDef.Object=old;}
const CDefinitionManager::CDefClassInfo* CDefinitionManager::GetPDefClassInfo(const CCharString& name)const{good &= !strcmp(text(name),"CLASS");log('C');return test==3?0:classInfo;}
bool CResourceList::MakeFreeSpace(unsigned long size){good &= size==137;log('M');return test!=6;}
void CResourceList::FrameEnd(){++CurrentFrame;log('F');}
void CResourceList::AddResource(CResource& resource){good &= resource.ResourceSize==137;resource.ResourceList=this;resource.LastUsedFrame=CurrentFrame;log('R');}
// Keep the callback provider emitted after the original inline declaration recovery.
void (CResourceList::*DefinitionLoadAddResourceProvider)(CResource&) = &CResourceList::AddResource;
void CResource::ListRemove(){log('L');}
void CResource::ListInsert(CResource* prev,CResource* next){good &= prev==(CResource*)0x1234&&next==&manager->Head;log('V');}
static CDefClassBase* FABLE_FASTCALL allocate(){log('A');return definition;}
static void FABLE_FASTCALL setName(void* self,void*,CDefString name){good &= self==definition&&name.TablePos==17;log('N');}
static void FABLE_FASTCALL internalLoad(void* self,void*,CDataInputStream& stream){good &= self==definition&&stream.GetLength()==100&&definition->PDefManager==manager&&definition->GlobalIndex==1;log('I');}
static void FABLE_FASTCALL transfer(void* self,void*,CPersistContext& context){good &= self==definition&&context.Mode==CPersistContext::MODE_BINARY_LOAD&&context.PLoadStream->GetLength()==100&&!context.PDefinitionManager&&!context.UseCRCCheck;log('T');}
struct View:CDefinitionManager{static void setup(CDefinitionManager* m,CInstantiatedDefInfo** slots){View* p=static_cast<View*>(m);p->InstantiatedDefs._M_start=slots;p->UseSafeBinaryFormat=true;}static void run(CDefinitionManager* m){static_cast<View*>(m)->LoadDef(1);}};
int main(){void* vtable[25]={0};vtable[11]=(void*)setName;vtable[22]=(void*)internalLoad;vtable[18]=(void*)transfer;
 for(test=0;test<7;++test){unsigned long managerRaw[52]={0},entryRaw[4]={0},infoRaw[5]={0},defRaw[10]={0},oldRaw[10]={0};manager=(CDefinitionManager*)managerRaw;entry=(CDefinitionManager::CInstantiatedDefInfo*)entryRaw;classInfo=(CDefinitionManager::CDefClassInfo*)infoRaw;definition=(CDefClassBase*)defRaw;old=(CDefClassBase*)oldRaw;defRaw[0]=oldRaw[0]=(unsigned long)vtable;defRaw[1]=1;oldRaw[1]=2;entry->ClassName.TablePos=100;entry->InstantiationName.TablePos=17;entry->PDef.Object=test<3?definition:0;classInfo->AllocFunc=allocate;CDefinitionManager::CInstantiatedDefInfo* slots[2]={0,entry};View::setup(manager,slots);manager->CurrentFrame=7;manager->Head.PrevResource=(CResource*)0x1234;if(test==1||test==2){definition->ResourceList=manager;definition->LastUsedFrame=test==1?7:3;}good=true;used=destructs=0;events[0]=0;View::run(manager);
 if(!good)return 1;
 if(test<3){if(strcmp(events,test==2?"LV":"")||destructs)return 2;if(test==2&&definition->LastUsedFrame!=7)return 3;}
 else if(test==3){if(strcmp(events,"1DC")||destructs!=1||entry->PDef.Get())return 4;}
 else if(strcmp(events,"1DCMFANI2TR3")||destructs!=1||entry->PDef.Get()!=definition||definition->IVRefCount!=1||definition->ResourceSize!=137||definition->LastUsedFrame!=8||old->IVRefCount!=(test==5?1:2))return 5;}
 puts("DEFINITION_LOAD_OK cases=7");return 0;}
