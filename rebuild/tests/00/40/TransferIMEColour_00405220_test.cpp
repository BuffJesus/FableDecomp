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

const CCharString& CCharString::operator+=(const char* suffix){char result[256];strcpy(result,text(*this));strcat(result,suffix);*this=result;return *this;}
unsigned char g_IMEAlphaDefault_013750B0=255;
unsigned long FABLE_FASTCALL TransferIMEColour(CPersistContext&,const CCharString&,unsigned long);
static unsigned calls;static signed char seen[4],defaults[4];static const char* names[]={"ColourR","ColourG","ColourB","ColourA"};static bool replace;
void CPersistContext::TransferSignedChar(const char* name,signed char& value,const signed char& def){if(strcmp(name,names[calls]))abort();seen[calls]=value;defaults[calls]=def;if(replace)value=static_cast<signed char>(17+calls*34);++calls;}
int main(){for(unsigned i=0;i<2;++i){unsigned long raw[11]={0};CPersistContext* c=(CPersistContext*)raw;calls=0;replace=i!=0;unsigned long result=TransferIMEColour(*c,CCharString("Colour"),0x12345678);
 if(calls!=4||static_cast<unsigned char>(seen[0])!=0x34||static_cast<unsigned char>(seen[1])!=0x56||static_cast<unsigned char>(seen[2])!=0x78||static_cast<unsigned char>(seen[3])!=255||memcmp(seen,defaults,4)||result!=(i?0xff113355UL:0xff345678UL))return 1;}
 puts("IME_COLOUR_OK cases=2");return 0;}
