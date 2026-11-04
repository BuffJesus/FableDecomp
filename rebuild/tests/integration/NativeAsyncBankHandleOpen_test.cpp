#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_retail_banks.h"
#include "fable_game_text_bank.h"
#include "fable_progress_display.h"
#include "test_support/base_class_stubs.h"
static void require(bool ok,int line){if(!ok){fprintf(stderr,"ASYNC_HANDLE_FAILURE line=%d\n",line);exit(80);}}
#define CHECK(x) require((x)!=0,__LINE__)
extern "C" unsigned FixtureManagerWords[sizeof(CBankFileManager)/4]={0};
#pragma comment(linker,"/alternatename:?g_FableBankFileManager_013CA79C@@3VCBankFileManager@@A=_FixtureManagerWords")
static CBankFileAsync* current;static const CCharString* argument;static unsigned flags;
static bool baseResult,retailMode,found,openResult;static unsigned baseCalls,progressCalls,findCalls,pathCalls,wideDrops,threadCtors,threadDtors,openCalls,oldDrops;
static unsigned retailWords[10],threadWords[7],oldWords[7];
static void __fastcall UnexpectedDelete(void*){CHECK(false);}
static void __fastcall OldDelete(void* p){CHECK(p==oldWords);++oldDrops;}
static CCPPointerInfo retailInfo(UnexpectedDelete,retailWords),threadInfo(UnexpectedDelete,threadWords);
static const wchar_t pathToken[]=L"controlled-async-dev.wad";
CBaseClassNonCopyable::CBaseClassNonCopyable(){}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CCharString::CCharString():storage_(0){}
CCharString::CCharString(const char* p,long):storage_(reinterpret_cast<CCharStringData*>(const_cast<char*>(p))){}
CCharString::~CCharString(){storage_=0;}
CCharString::operator const char*()const{return reinterpret_cast<const char*>(storage_);}
CWideString::CWideString():storage_(0){}
CWideString::CWideString(const wchar_t* p):storage_(reinterpret_cast<CWideStringData*>(const_cast<wchar_t*>(p))){}
CWideString::~CWideString(){storage_=0;++wideDrops;}
CWideString::operator const wchar_t*()const{return reinterpret_cast<const wchar_t*>(storage_);}
bool CBankFile::OpenReadOnly(const CCharString& handle,unsigned long f){CHECK(this==current && &handle==argument && f==flags);++baseCalls;RetailMode=retailMode;return baseResult;}
CWideString CBankFile::GetBankPathname()const{CHECK(this==current && threadCtors==1 && current->ThreadedFile.control_->RefCount==1);++pathCalls;return CWideString(pathToken);}
void __fastcall NProgressDisplay::DisplayProgress(const CCharString& text,float fraction,bool force,bool debug){CHECK(baseCalls==1 && !strcmp((const char*)text,"Open Bank File Async") && fraction==-1.0f && !force && !debug);++progressCalls;}
bool CBankFileManager::FindContainedBank(const CCharString& handle,NBankFileManager::CContainedBank& info,CCountedPointer<NBankFileManager::CRetailBank>& owner){
 CHECK((void*)this==FixtureManagerWords && &handle==argument && progressCalls==1 && !owner.Data && !owner.Info);unsigned char zero[20]={0};CHECK(!memcmp(&info,zero,20));++findCalls;if(!found)return false;owner.Data=(NBankFileManager::CRetailBank*)retailWords;owner.Info=&retailInfo;++retailInfo.RefCount;return true;
}
CThreadedFile::CThreadedFile(){++threadCtors;}
CThreadedFile::~CThreadedFile(){++threadDtors;}
bool CThreadedFile::Open(const CWideString& path,bool noCache){CHECK(this==current->ThreadedFile.data_ && (const wchar_t*)path==pathToken && noCache==current->NonCached && pathCalls==1);++openCalls;return openResult;}
void __fastcall CCountedPointer<CThreadedFile>::DeleteData(void* p){delete (CThreadedFile*)p;}
int main(){
 unsigned baseFailures=0,missing=0,retail=0,dev=0,same=0;
 for(unsigned test=0;test<512;++test){
  unsigned words[sizeof(CBankFileAsync)/4];memset(words,0xa9,sizeof(words));current=(CBankFileAsync*)words;CCharString input("async-bank-token");argument=&input;flags=test*0x12345u;
  baseResult=(test&1)!=0;retailMode=(test&2)!=0;found=(test&4)!=0;openResult=(test&8)!=0;bool sameOwner=baseResult && retailMode && found && (test&16);
  baseCalls=progressCalls=findCalls=pathCalls=wideDrops=threadCtors=threadDtors=openCalls=oldDrops=0;
  memset(retailWords,0,sizeof(retailWords));retailWords[3]=(unsigned)threadWords;retailWords[4]=(unsigned)&threadInfo;retailWords[5]=(test&32)?1:0;retailInfo.RefCount=1;threadInfo.RefCount=sameOwner?2:1;
  current->ThreadedFile.data_=(CThreadedFile*)(sameOwner?threadWords:oldWords);current->ThreadedFile.control_=sameOwner?&threadInfo:new CCPPointerInfo(OldDelete,oldWords);current->NonCached=(test&64)!=0;current->DisableAsync=true;
  bool initialCache=current->NonCached;CCPPointerInfo* initialOwner=current->ThreadedFile.control_;bool result=current->CBankFileAsync::OpenReadOnly(input,flags);
  CHECK(result==(baseResult && (!retailMode || found)) && baseCalls==1 && retailInfo.RefCount==1);
  if(!baseResult){++baseFailures;CHECK(!progressCalls && !findCalls && !threadCtors && !openCalls && current->DisableAsync && current->NonCached==initialCache && current->ThreadedFile.control_==initialOwner);}
  else if(retailMode && !found){++missing;CHECK(progressCalls==1 && findCalls==1 && !threadCtors && !openCalls && current->DisableAsync && current->NonCached==initialCache && current->ThreadedFile.control_==initialOwner);}
  else if(retailMode){++retail;if(sameOwner)++same;CHECK(progressCalls==1 && findCalls==1 && !threadCtors && !openCalls && !current->DisableAsync && current->NonCached==((test&32)!=0) && current->ThreadedFile.data_==(CThreadedFile*)threadWords && current->ThreadedFile.control_==&threadInfo && threadInfo.RefCount==2 && oldDrops==(sameOwner?0:1));}
  else{++dev;CHECK(progressCalls==1 && !findCalls && threadCtors==1 && openCalls==1 && pathCalls==1 && wideDrops==1 && current->NonCached==initialCache && !current->DisableAsync && oldDrops==1 && current->ThreadedFile.control_->RefCount==1);}
  current->ThreadedFile.~CCountedPointer();CHECK(threadInfo.RefCount==1 && oldDrops==(sameOwner?0:1) && threadDtors==((baseResult && !retailMode)?1:0));
 }
 CHECK(baseFailures==256 && missing==64 && retail==64 && dev==128 && same==32);printf("NATIVE_ASYNC_HANDLE_PASS cases=512 base_fail=256 missing=64 retail=64 dev=128 same_owner=32\n");return 0;
}
