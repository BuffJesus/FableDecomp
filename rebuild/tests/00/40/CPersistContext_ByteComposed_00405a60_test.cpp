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

CCharString FABLE_FASTCALL operator+(const char* a,const CCharString& b){char s[256];strcpy(s,a);strcat(s,text(b));return CCharString(s);}
static int errors,parserErrors;static char message[256];
void CPersistContext::PersistError(const CStringParser&,const CCharString& msg) const{++errors;strcpy(message,text(msg));}
void CStringParser::Error(const CCharString&){++parserErrors;}
bool CSymbolMap::FindSymbol(const CCharString& name,long& val)const{if(!strcmp(text(name),"Known")){val=5;return true;}return false;}
template<> void FABLE_FASTCALL _STL::sort(_STL::pair<CCharString,CSymbolMap::CSymbolInfo>* first,_STL::pair<CCharString,CSymbolMap::CSymbolInfo>* last,CCompareString<CSymbolMap::CSymbolInfo> compare){}
CBaseClass::~CBaseClass(){}
const CCharString& CCharString::operator+=(const char* suffix){char result[256];strcpy(result,text(*this));strcat(result,suffix);*this=result;return *this;}
const CCharString& CCharString::operator+=(const CCharString& suffix){return *this+=text(suffix);}
CCharString FABLE_FASTCALL GFIntToCharString(long value){char s[32];sprintf(s,"%ld",value);return CCharString(s);}
void CPersistContext::CheckCRC(const char*){}
void CDataOutputStream::WriteSLONG(long){abort();}
void CPersistTraits<unsigned char>::TransferBinaryIn(CMemoryDataInputStream&,unsigned char&){abort();}
int main(){const char* sources[]={"Byte 255;","Byte 1<<8;","Byte Known+1;","Byte -1;","Other 1;","Byte 1; Byte 2;","Other 1;"};unsigned expected[]={255,0,6,255,7,2,128};
 for(unsigned i=0;i<7;++i){unsigned long raw[10]={0},snapshot[10],contextRaw[11]={0},mapRaw[9]={0};CStringParser* p=(CStringParser*)raw;CPersistContext* c=(CPersistContext*)contextRaw;CCharString clean(sources[i]);p->String=text(clean);typedef std::_List_node<CStringParserCommentBoundaries> Node;Node* head=(Node*)calloc(1,sizeof(Node));head->_M_next=head;head->_M_prev=head;raw[5]=(unsigned long)head;memcpy(snapshot,raw,sizeof(raw));c->PParser=p;c->PSymbolMap=(CSymbolMap*)mapRaw;c->PStringToClean=&clean;c->UseDefaults=i!=6;c->Mode=CPersistContext::MODE_TEXT_LOAD;signed char value=-128,def=7;errors=parserErrors=0;c->TransferSignedChar("Byte",value,def);
 if((unsigned char)value!=expected[i]||memcmp(snapshot,raw,sizeof(raw))||errors||parserErrors){printf("case %u value %u errors %d parser %d\n",i,(unsigned char)value,errors,parserErrors);return 1;}free(head);}
 unsigned long raw[11]={0};CPersistContext* c=(CPersistContext*)raw;CCharString output;c->Mode=CPersistContext::MODE_TEXT_SAVE;c->PStringToSaveTo=&output;signed char value=-128,def=0;c->TransferSignedChar("Byte",value,def);if(strcmp(text(output),"Byte 128;\r\n"))return 2;
 puts("PERSIST_BYTE_COMPOSED_OK cases=8");return 0;}
