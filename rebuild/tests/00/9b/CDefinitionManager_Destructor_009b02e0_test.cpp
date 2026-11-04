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
static unsigned cleared,baseDestroyed,wideDestroyed;
CWideString::~CWideString(){++wideDestroyed;}
CResourceList::~CResourceList(){++baseDestroyed;}
bool CResourceList::HandleFailedAllocation(unsigned long){return false;}
void CDefinitionManager::ClearInstantiatedDefs(){++cleared;}
void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase()const{abort();}
void CResourceList::RemoveResource(CResource&){}
static unsigned deleted;
void FABLE_FASTCALL drop(void*){++deleted;}
struct View:CDefinitionManager{using CDefinitionManager::RecompiledDefs;};
int main(){for(unsigned nodes=0;nodes<4;++nodes){unsigned long raw[52]={0};View* manager=(View*)raw;typedef _STL::pair<const unsigned long,CDefPointer<CDefClassBase> > Entry;typedef _STL::_Rb_tree_node<Entry> Node;Node* head=(Node*)calloc(1,sizeof(Node));head->_M_left=head;head->_M_right=head;manager->RecompiledDefs._M_t._M_header._M_data=head;manager->RecompiledDefs._M_t._M_node_count=nodes;unsigned long pointees[3][7]={0};Node* prior=0;
 for(unsigned i=0;i<nodes;++i){Node* node=(Node*)calloc(1,sizeof(Node));if(i==2)node->_M_right=prior;else node->_M_left=prior;node->_M_value_field.second.Object=pointees[i];pointees[i][1]=2;prior=node;}head->_M_parent=prior;
 CCPPointerInfo* info=new CCPPointerInfo(drop,0);raw[22]=1;raw[23]=(unsigned long)info;cleared=baseDestroyed=wideDestroyed=deleted=0;manager->CDefinitionManager::~CDefinitionManager();if(cleared!=1||baseDestroyed!=1||wideDestroyed!=1||deleted!=1||raw[22]||raw[23])return 1;for(unsigned i=0;i<nodes;++i)if(pointees[i][1]!=1)return 2;}
 puts("DEFINITION_DTOR_OK cases=4");return 0;}

