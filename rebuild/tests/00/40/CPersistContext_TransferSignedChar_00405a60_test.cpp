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
static unsigned crcCalls,saveCalls,loadCalls,binaryCalls;static long written;
void CPersistContext::CheckCRC(const char* name){if(strcmp(name,"Byte"))abort();++crcCalls;}
void CDataOutputStream::WriteSLONG(long value){++binaryCalls;written=value;}
void CPersistTraits<unsigned char>::TransferBinaryIn(CMemoryDataInputStream&,unsigned char& value){++binaryCalls;value=201;}
void FABLE_FASTCALL CPersistTraitsSLONG::TransferIn(CPersistContext&,CStringParser&,long& value){value=511;}
CCharString FABLE_FASTCALL GFIntToCharString(long value){char s[32];sprintf(s,"%ld",value);return CCharString(s);}
const CCharString& CCharString::operator+=(const CCharString& suffix){return *this+=text(suffix);}
void CPersistContext::TransferObjectSaveText(const char*,CPersistContext_TransferableComponentGenericType& object){++saveCalls;CCharString output;object.TransferOut(*this,output);if(strcmp(text(output),"128"))abort();}
void CPersistContext::TransferObjectLoadText(const char*,CPersistContext_TransferableComponentGenericType& object){++loadCalls;CStringParser* parser=(CStringParser*)PParser;object.TransferIn(*this,*parser);}
int main(){for(unsigned mode=0;mode<5;++mode){unsigned long raw[11]={0},parserRaw[10]={0};CPersistContext* c=(CPersistContext*)raw;c->Mode=(CPersistContext::EMode)mode;c->PParser=(CStringParser*)parserRaw;c->UseDefaults=true;signed char value=-128,def=7;crcCalls=saveCalls=loadCalls=binaryCalls=0;written=0;c->TransferSignedChar("Byte",value,def);
 unsigned expected[]={255,128,201,128,7};if(static_cast<unsigned char>(value)!=expected[mode]||crcCalls!=1||saveCalls!=(mode==1)||loadCalls!=(mode==0)||binaryCalls!=(mode==2||mode==3)||(mode==3&&written!=128)){printf("mode %u value %u\n",mode,(unsigned char)value);return 1;}}
 puts("PERSIST_BYTE_DISPATCH_OK cases=5");return 0;}
