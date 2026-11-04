#pragma once
#include "fable_string.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
static unsigned liveData,copyCalls,extendCalls;
static void require(bool value){if(!value){puts("string fixture mismatch");exit(1);}}
template<> CBasicString<char>::CBasicString(const char* text){
 StringLength=strlen(text);DataLength=(StringLength+4)&~3;UseFastExtend=true;
 PData=static_cast<char*>(malloc(DataLength));memset(PData,0,DataLength);memcpy(PData,text,StringLength);++liveData;
}
template<> void CBasicString<char>::ExtendData(unsigned long length){
 ++extendCalls;unsigned long capacity=(length+4)&~3;
 if(capacity>DataLength){PData=static_cast<char*>(realloc(PData,capacity));memset(PData+DataLength,0xA5,capacity-DataLength);DataLength=capacity;}
}
CCharString::CCharString():storage_(0){}
CCharString::CCharString(const char* text,long length):storage_(0){require(length==-1);if(text && *text)storage_=new CCharStringData(text);}
CCharString::CCharString(const CCharString& other):storage_(other.storage_){if(storage_)++storage_->owners;}
CCharString::~CCharString(){if(storage_ && --storage_->owners==0){free(storage_->text);delete storage_;--liveData;}}
void CCharString::MakeOwnCopyOfData(){++copyCalls;require(storage_!=0);if(storage_->owners>1){CCharStringData* old=storage_;storage_=new CCharStringData(old->text);--old->owners;}}
static CCharStringData* dataOf(const CCharString& text){return *reinterpret_cast<CCharStringData* const*>(&text);}
