#include "fable_definition_manager.h"
#include "fable_string_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static char*& slot(const CCharString& s) { return *reinterpret_cast<char**>(const_cast<CCharString*>(&s)); }
static const char* text(const CCharString& s) { return slot(s) ? slot(s) : ""; }
static char* copy(const char* s) { char* p=(char*)malloc(strlen(s)+1); strcpy(p,s); return p; }
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CCharString::CCharString() : storage_(0) {}
CCharString::CCharString(const char* s,long) : storage_(0) { slot(*this)=copy(s); }
CCharString::CCharString(const CCharString& s) : storage_(0) { slot(*this)=copy(text(s)); }
CCharString::~CCharString() { free(slot(*this)); }
CCharString FABLE_FASTCALL operator+(const char* left,const CCharString& right) { char b[256]; strcpy(b,left); strcat(b,text(right)); return CCharString(b); }
static int records,errors,enums,resets; static bool good;
void CStringParser::Init(const char* s,ErrorCallbackFunction,const CCharString&,long n) { good &= n==0; String=s;StringPos=0; }
bool CStringParser::SkipPastWholeString(const CCharString& word) { char* p=strstr(String+StringPos,text(word)); while(p && p[strlen(text(word))]=='_')p=strstr(p+1,text(word)); if(!p)return false; StringPos=(unsigned long)(p-String)+strlen(text(word)); return true; }
void CStringParser::SetStringPos(unsigned long n) { good &= n==0;StringPos=n;++resets; }
CCharString CStringParser::ReadAsString() { char b[64];unsigned i=0;while(String[StringPos]==' ')++StringPos;while(String[StringPos] && String[StringPos]!=' ' && String[StringPos]!=';')b[i++]=String[StringPos++];b[i]=0;return CCharString(b); }
bool CDefinitionManager::InstantiateDef(const CCharString& cls,const CCharString& name,bool flag) { ++records;good &= (flag ? !strcmp(text(cls),"CREATURE") && !strcmp(text(name),"BASE") : !strcmp(text(cls),"ENGINE") && !strcmp(text(name),"ENGINE"));return flag; }
void CDefinitionManager::Error(const CStringParser&,const CCharString& message) { ++errors;good &= !strcmp(text(message),"Unable to find def class called ENGINE"); }
void CDefinitionManager::ParseEnumForSymbols(CStringParser&) { ++enums; }
struct View : CDefinitionManager { static void run(CDefinitionManager* p,char* s,bool flag) { static_cast<View*>(p)->ParseStringForSymbols(s,flag); } };
int main() {
 unsigned long raw[52]={0};CDefinitionManager* m=reinterpret_cast<CDefinitionManager*>(raw);
 char source[]="enum E; #definition ENGINE ENGINE; #definition_template CREATURE BASE;";
 good=true;View::run(m,source,true);
 if(!good || records!=2 || errors!=1 || enums!=1 || resets!=2)return 1;
 records=errors=enums=resets=0;View::run(m,source,false);
 if(!good || records || errors || enums!=1 || resets)return 2;
 puts("DEFINITION_SYMBOL_PARSER_OK cases=2");return 0;
}
