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
void CStringParser::Error(const CCharString&){++count;}
struct Adapter:CPersistContext_TransferableComponentGenericType{int calls;Adapter():calls(0){}void TransferOut(CPersistContext&,CCharString&){}void TransferIn(CPersistContext&,CStringParser& parser){++calls;parser.ReadAsString();}};
struct Access:CPersistContext{static void run(CPersistContext* c,Adapter& adapter){static_cast<Access*>(c)->TransferObjectLoadText("Enabled",adapter);}};
int main(){const char* sources[]={"Enabled TRUE; Enabled FALSE;","Other TRUE;","Enabled TRUE; Enabled FALSE;","Enabled TRUE;","Enabled TRUE ? Enabled FALSE;","Enabled\n TRUE;"};int expected[]={2,0,1,0,2,1};
 for(unsigned i=0;i<6;++i){unsigned long raw[10]={0},snapshot[10],contextRaw[11]={0};CStringParser* p=(CStringParser*)raw;CPersistContext* c=(CPersistContext*)contextRaw;CCharString clean(sources[i]);p->String=text(clean);typedef std::_List_node<CStringParserCommentBoundaries> Node;Node* head=(Node*)calloc(1,sizeof(Node));head->_M_next=head;head->_M_prev=head;raw[5]=(unsigned long)head;memcpy(snapshot,raw,sizeof(raw));c->PParser=p;c->PStringToClean=&clean;c->SingleTransfer=i==2;c->Finished=i==3;count=0;Adapter adapter;Access::run(c,adapter);
 if(adapter.calls!=expected[i]||memcmp(snapshot,raw,sizeof(raw))||count!=(i==4?1:0)){printf("case %u calls %d errors %d\n",i,adapter.calls,count);return 1;}
 if(i==0){for(unsigned j=0;j<strlen(sources[i]);++j)if(text(clean)[j]!=' ')return 2;}
 if(i==5&&text(clean)[7]!='\n')return 3;
 if(c->Finished!=(i==2||i==3))return 4;free(head);}
 puts("PERSIST_TEXT_LOAD_OK cases=6");return 0;}
