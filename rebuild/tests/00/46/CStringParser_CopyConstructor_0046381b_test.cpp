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

#include <new>
CBaseClass::~CBaseClass(){}
static void FABLE_FASTCALL callback(const CCharString&){}
typedef std::_List_node<CStringParserCommentBoundaries> Node;
int main(){for(unsigned n=0;n<4;n+=(n==1?2:1)){
 unsigned long raw[10]={0},dest[10]={0};CStringParser* source=(CStringParser*)raw;source->String="input";source->BaseLineOffset=123;slot(source->Filename)=make("file.def",-1);slot(source->TempCharString)=make("temp",-1);source->ErrorCallback=callback;raw[7]=0xa1b2c301;raw[8]=9;raw[9]=27;
 Node* sentinel=(Node*)calloc(1,sizeof(Node));sentinel->_M_next=sentinel;sentinel->_M_prev=sentinel;raw[5]=(unsigned long)sentinel;
 for(unsigned i=0;i<n;++i){Node* node=(Node*)malloc(sizeof(Node));node->_M_data.PStart=(char*)(0x12340000+i*10);node->_M_data.PEnd=node->_M_data.PStart+7;node->_M_next=sentinel;node->_M_prev=sentinel->_M_prev;sentinel->_M_prev->_M_next=node;sentinel->_M_prev=node;}
 CStringParser* copy=new(dest)CStringParser(*source);if(copy->String!=source->String || copy->BaseLineOffset!=123 || strcmp(text(copy->Filename),"file.def") || strcmp(text(copy->TempCharString),"temp") || copy->ErrorCallback!=callback || dest[7]!=raw[7] || dest[8]!=9 || dest[9]!=27)return 1;
 Node* other=(Node*)dest[5];if(other==sentinel)return 2;Node* a=sentinel->_M_next;Node* b=other->_M_next;
 for(unsigned i=0;i<n;++i){if(a==sentinel||b==other||a==b||a->_M_data.PStart!=b->_M_data.PStart||a->_M_data.PEnd!=b->_M_data.PEnd||b->_M_next->_M_prev!=b)return 3;a=a->_M_next;b=b->_M_next;}if(a!=sentinel||b!=other)return 4;
 copy->~CStringParser();if(sentinel->_M_prev->_M_next!=sentinel)return 5;
 a=sentinel->_M_next;while(a!=sentinel){Node* next=a->_M_next;free(a);a=next;}free(sentinel);release(slot(source->Filename));release(slot(source->TempCharString));}
 puts("PARSER_COPY_OK cases=3");return 0;}
