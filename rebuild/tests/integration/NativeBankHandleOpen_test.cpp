#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_retail_banks.h"
#include "fable_game_text_bank.h"
#include "fable_data_input_stream.h"
#include "test_support/base_class_stubs.h"
#include "test_support/data_input_stream_stubs.h"
static void require(bool ok,int line){if(!ok){fprintf(stderr,"BANK_HANDLE_FAILURE line=%d\n",line);exit(80);}}
#define CHECK(x) require((x)!=0,__LINE__)
extern "C" unsigned FixtureManagerWords[sizeof(CBankFileManager)/4]={0};
#pragma comment(linker,"/alternatename:?g_FableBankFileManager_013CA79C@@3VCBankFileManager@@A=_FixtureManagerWords")
static CBankFile* current;static const CCharString* argument;static unsigned flags;
static bool found,versionMatches,leafResult,devResult;static unsigned findCalls,formatCalls,readCalls,openedCalls,devCalls,pathCalls,streamCtor,streamDtor,wideDtor;
static NBankFileManager::CContainedBank supplied;static unsigned retailWords[10],diskWords[7];
static void __fastcall UnexpectedDelete(void*){CHECK(false);}
static CCPPointerInfo retailInfo(UnexpectedDelete,retailWords),diskInfo(UnexpectedDelete,diskWords);
static const wchar_t pathToken[]=L"controlled-dev-bank.wad";
CBaseClassNonCopyable::CBaseClassNonCopyable(){}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CCharString::CCharString():storage_(0){}
CCharString::CCharString(const char* value,long):storage_(reinterpret_cast<CCharStringData*>(const_cast<char*>(value))){}
CCharString::CCharString(const CCharString& other):storage_(other.storage_){}
CCharString::~CCharString(){storage_=0;}
CCharString& CCharString::operator=(const CCharString& other){storage_=other.storage_;return *this;}
CCharString::operator const char*()const{return reinterpret_cast<const char*>(storage_);}
CWideString::CWideString():storage_(0){}
CWideString::CWideString(const wchar_t* value):storage_(reinterpret_cast<CWideStringData*>(const_cast<wchar_t*>(value))){}
CWideString::CWideString(const CWideString& other):storage_(other.storage_){}
CWideString::~CWideString(){storage_=0;++wideDtor;}
CWideString::operator const wchar_t*()const{return reinterpret_cast<const wchar_t*>(storage_);}
bool CBankFileManager::FindContainedBank(const CCharString& name,NBankFileManager::CContainedBank& info,CCountedPointer<NBankFileManager::CRetailBank>& result){
 CHECK((void*)this==FixtureManagerWords && &name==argument && !result.Data && !result.Info);++findCalls;
 unsigned char zero[20]={0};CHECK(!memcmp(&info,zero,20));if(!found)return false;info=supplied;result.Data=reinterpret_cast<NBankFileManager::CRetailBank*>(retailWords);result.Info=&retailInfo;++retailInfo.RefCount;return true;
}
CWideString CBankFileManager::GetDevBankFilePath(const CCharString& name)const{CHECK((void*)this==FixtureManagerWords && &name==argument);++pathCalls;return CWideString(pathToken);}
CDataInputStream::CDataInputStream(){}
static CFileDataInputStream* activeStream;
CFileDataInputStream::CFileDataInputStream(CAFile* file,unsigned long size){CHECK((void*)file==diskWords && size==0x4000 && diskInfo.RefCount==2);File=file;activeStream=this;++streamCtor;}
CFileDataInputStream::~CFileDataInputStream(){CHECK(this==activeStream && openedCalls==1 && retailInfo.RefCount==2);++streamDtor;}
void CDataInputStream::SetPosition(unsigned long position){CHECK(this==activeStream && position==supplied.infoOffset_);StreamPos=position;}
bool CBankFile::ReadEntryList(CFileDataInputStream& stream,unsigned long maximum,unsigned long count){CHECK(this==current && &stream==activeStream && stream.GetPosition()==supplied.infoOffset_ && maximum==count+1 && count==supplied.entryCount_ && Alignment==supplied.alignment_);++readCalls;FileValid=leafResult;return leafResult;}
static unsigned long __fastcall Format(CBankFile* self,void*){CHECK(self==current);++formatCalls;return versionMatches?supplied.dataVersion_:supplied.dataVersion_^1;}
static void __fastcall Opened(CBankFile* self,void*){CHECK(self==current && readCalls==1 && !streamDtor);++openedCalls;}
static bool __fastcall OpenFile(CBankFile* self,void*,const CWideString& name,unsigned long inputFlags){CHECK(self==current && inputFlags==flags && (const wchar_t*)name==pathToken && pathCalls==1);++devCalls;return devResult;}
int main(){
 unsigned state=0x12345678,retailCases=0,missingCases=0,versionCases=0,devCases=0;
 for(unsigned test=0;test<512;++test){
  unsigned words[sizeof(CBankFile)/4];memset(words,0xa7,sizeof(words));current=reinterpret_cast<CBankFile*>(words);void* table[20]={0};table[3]=(void*)OpenFile;table[8]=(void*)Format;table[11]=(void*)Opened;words[0]=(unsigned)table;
  memset((char*)words+0x7c,0,8);current->RetailMode=false;current->ReadOnly=false;current->WrittenToFile=current->Solid=current->UpdatingFlag=current->FileValid=true;
  CCharString input("bank-token");argument=&input;new(&current->BankHandle) CCharString("stale-token");
  state=state*1664525+1013904223;flags=(state&~4u)|((test&2)?4:0);reinterpret_cast<CBankFileManager*>(FixtureManagerWords)->UseRetailBanks=(test&1)!=0;
  found=(test&4)!=0;versionMatches=(test&8)!=0;leafResult=(test&16)!=0;devResult=(test&32)!=0;
  supplied.dataVersion_=100+test%33;supplied.entryCount_=test%65;supplied.infoOffset_=0x2340+test*16;supplied.infoSize_=99;supplied.alignment_=1u<<((test>>6)%12);
  memset(retailWords,0,sizeof(retailWords));retailWords[1]=(unsigned)diskWords;retailWords[2]=(unsigned)&diskInfo;retailInfo.RefCount=diskInfo.RefCount=1;
  findCalls=formatCalls=readCalls=openedCalls=devCalls=pathCalls=streamCtor=streamDtor=wideDtor=0;activeStream=0;
  bool route=(test&1) && !(flags&4);bool success=route?found&&versionMatches:devResult;bool result=current->CBankFile::OpenReadOnly(input,flags);
  CHECK(result==success && current->ReadOnly && current->OpenFlags==flags && !strcmp((const char*)current->BankHandle,"bank-token") && !current->ChangeCount && !current->WrittenToFile && !current->Solid && !current->UpdatingFlag && current->FileSize==0xa7a7a7a7 && retailInfo.RefCount==1);
  if(route){CHECK(current->RetailMode && findCalls==1 && formatCalls==(found?1:0) && !devCalls && !pathCalls && !wideDtor);
   if(success){++retailCases;CHECK(readCalls==1 && openedCalls==1 && streamCtor==1 && streamDtor==1 && diskInfo.RefCount==2 && current->BankFile.Info==&diskInfo && current->BankFile.Data==(CDiskFileWin32*)diskWords && current->Alignment==supplied.alignment_ && current->FileValid==leafResult);}
   else{if(found)++versionCases;else ++missingCases;CHECK(!readCalls && !openedCalls && !streamCtor && !streamDtor && !current->FileValid && !current->BankFile.Info && diskInfo.RefCount==1 && current->Alignment==0xa7a7a7a7);}
  }else{++devCases;CHECK(!current->RetailMode && !findCalls && !formatCalls && !readCalls && !openedCalls && devCalls==1 && pathCalls==1 && wideDtor==1 && !current->FileValid && !current->BankFile.Info && diskInfo.RefCount==1);}
  current->BankFile.~CCountedPointer();current->BankHandle.~CCharString();CHECK(diskInfo.RefCount==1);
 }
 CHECK(retailCases==32 && missingCases==64 && versionCases==32 && devCases==384);printf("NATIVE_BANK_HANDLE_PASS cases=512 retail=32 missing=64 version=32 dev=384\n");return 0;
}
