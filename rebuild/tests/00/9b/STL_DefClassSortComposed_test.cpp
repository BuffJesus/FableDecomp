#include <new>
#include "fable_definition_manager.h"
#include "fable_stlport_vector_impl.h"
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

const CCharString& CCharString::operator+=(const char* suffix){char result[256];strcpy(result,text(*this));strcat(result,suffix);*this=result;return *this;}
typedef _STL::pair<unsigned long,CDefinitionManager::CDefClassInfo> Entry;
typedef CKeyPairCompareLess<unsigned long,CDefinitionManager::CDefClassInfo> Less;
static bool valid(Entry* first,unsigned length){for(unsigned i=0;i<length;++i){char name[32];sprintf(name,"key%lu",first[i].first);if(strcmp(text(first[i].second.Name),name)||first[i].second.GlobalIndices.size()!=1||first[i].second.GlobalIndices[0]!=first[i].first+100)return false;}return true;}
static void fill(Entry* a,unsigned n){for(unsigned i=0;i<n;++i){a[i].first=i?((i*17)%31+1):0;char name[32];sprintf(name,"key%lu",a[i].first);a[i].second.Name=name;a[i].second.GlobalIndices.push_back(a[i].first+100);}}
static void initialise(Entry* a,unsigned n,unsigned mode){for(unsigned i=0;i<n;++i){a[i].first=mode==0?i:mode==1?n-i:(i*17)%31;char name[32];sprintf(name,"key%lu",a[i].first);a[i].second.Name=name;a[i].second.GlobalIndices.push_back(a[i].first+100);}}
static bool sorted(Entry* a,unsigned n){if(!valid(a,n))return false;for(unsigned i=1;i<n;++i)if(a[i-1].first>a[i].first)return false;return true;}
int main(){unsigned sizes[]={0,1,15,16,17,32,65,128};for(unsigned k=0;k<8;++k)for(unsigned mode=0;mode<3;++mode){Entry a[128];unsigned n=sizes[k];initialise(a,n,mode);_STL::sort(a,a+n,Less());if(!sorted(a,n))return 1;}
 for(unsigned n=17;n<=65;n+=24){Entry a[128];initialise(a,n,2);_STL::__introsort_loop(a,a+n,(Entry*)0,0,Less());if(!sorted(a,n))return 2;}
 puts("DEFCLASS_SORT_COMPOSED_OK cases=27");return 0;}
