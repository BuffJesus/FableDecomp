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

CBaseClass::~CBaseClass(){}
CCharString FABLE_FASTCALL operator+(const char* a,const CCharString& b){char s[256];strcpy(s,a);strcat(s,text(b));return CCharString(s);}
static int count;static char messages[4][256];
void CPersistContext::PersistError(const CStringParser&,const CCharString& message) const{strcpy(messages[count++],text(message));}
int main(){const char* sources[]={""," \t\n","Unknown","One  Two\nThree","\"Two Words\""};const int counts[]={0,0,1,3,2};const char* expected[5][3]={{0},{0},{"Unknown entry \"Unknown\""},{"Unknown entry \"One\"","Unknown entry \"Two\"","Unknown entry \"Three\""},{"Unknown entry \"\"Two\"","Unknown entry \"Words\"\""}};
 for(unsigned i=0;i<5;++i){unsigned long raw[10]={0},snapshot[10],contextRaw[11]={0};CStringParser* p=(CStringParser*)raw;CPersistContext* c=(CPersistContext*)contextRaw;p->String=sources[i];raw[7]=0xa1b2c300;raw[9]=17;slot(p->Filename)=make("file.def",-1);typedef std::_List_node<CStringParserCommentBoundaries> Node;Node* head=(Node*)calloc(1,sizeof(Node));head->_M_next=head;head->_M_prev=head;raw[5]=(unsigned long)head;memcpy(snapshot,raw,sizeof(raw));c->PParser=p;count=0;c->CheckUnknownSymbols();
 if(count!=counts[i]||memcmp(snapshot,raw,sizeof(raw)))return 1;for(int n=0;n<count;++n)if(strcmp(messages[n],expected[i][n]))return 2;release(slot(p->Filename));free(head);}
 puts("PERSIST_UNKNOWN_OK cases=5");return 0;}
