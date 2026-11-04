#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_string.h"
fable_i32 g_CCharStringInstanceCount_013BD800;
char g_FableEmptyNarrowString_0129AAF4[1]={0};
static bool failData;static int dataLive,buffersLive;
void* operator new(size_t n){if(n==sizeof(CCharStringData)){if(failData)return 0;++dataLive;}return malloc(n);}
void operator delete(void* p){if(p){--dataLive;free(p);}}
void* operator new[](size_t n){++buffersLive;return malloc(n);}
void operator delete[](void* p){if(p){--buffersLive;free(p);}}
template<> CBasicString<char>::CBasicString(const char* s){
 StringLength=(unsigned long)strlen(s);PData=new char[StringLength+1];memcpy(PData,s,StringLength+1);DataLength=StringLength+1;UseFastExtend=true;
}
CCharString::CCharString():storage_(0){++g_CCharStringInstanceCount_013BD800;}
CCharString::CCharString(const char* text,long):storage_(0){++g_CCharStringInstanceCount_013BD800;if(text&&*text)storage_=new CCharStringData(text);}
static CCharStringData* data(const CCharString& s){return *(CCharStringData* const*)&s;}
int main(){
 {CCharString empty;CCharString out=empty.GetAsLowerCase();if(data(out)||g_CCharStringInstanceCount_013BD800!=2)return 1;}
 {CCharString original("MiXeD Case 123!\tZ");CCharString out=original.GetAsLowerCase();
 if(strcmp(out.GetConstCharPtr(),"mixed case 123!\tz")||strcmp(original.GetConstCharPtr(),"MiXeD Case 123!\tZ"))return 2;
 if(data(out)==data(original)||data(out)->owners!=1||data(original)->owners!=1||g_CCharStringInstanceCount_013BD800!=2)return 3;
 }
 {CCharString original("ALREADY Shared");CCharString shared(original);CCharString out=shared.GetAsLowerCase();
 if(strcmp(out.GetConstCharPtr(),"already shared")||data(original)!=data(shared)||data(original)->owners!=2||data(out)->owners!=1)return 4;
 }
 {CCharString original("KEEP");failData=true;CCharString out=original.GetAsLowerCase();failData=false;
 if(data(out)||strcmp(original.GetConstCharPtr(),"KEEP")||data(original)->owners!=1)return 5;
 }
 if(g_CCharStringInstanceCount_013BD800||dataLive||buffersLive)return 6;
 puts("STRING_LOWER_PASS");return 0;
}