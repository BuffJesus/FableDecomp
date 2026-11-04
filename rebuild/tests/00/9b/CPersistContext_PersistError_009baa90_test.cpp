#include "fable_persist_context.h"
#include "fable_string_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static char*& slot(const CCharString& s){return *reinterpret_cast<char**>(const_cast<CCharString*>(&s));}
static const char* text(const CCharString& s){return slot(s)?slot(s):"";}
static char* copy(const char* s){char* p=(char*)malloc(strlen(s)+1);strcpy(p,s);return p;}
CCharString::CCharString(const char* s,long):storage_(0){slot(*this)=copy(s);}
CCharString::~CCharString(){free(slot(*this));}
CCharString FABLE_FASTCALL operator+(const CCharString& a,const char* b){char s[256];strcpy(s,text(a));strcat(s,b);return CCharString(s);}
CCharString FABLE_FASTCALL operator+(const CCharString& a,const CCharString& b){return a+text(b);}
CCharString FABLE_FASTCALL GFIntToCharString(long value){char s[32];sprintf(s,"%ld",value);return CCharString(s);}
static int calls;static char result[256];
static void FABLE_FASTCALL callback(const CCharString& message){++calls;strcpy(result,text(message));}
struct View:CPersistContext{static void report(CPersistContext* m,CStringParser& p,const CCharString& msg){static_cast<View*>(m)->PersistError(p,msg);}static void setup(CPersistContext* m){static_cast<View*>(m)->ErrorCallback=callback;}};
int main(){unsigned long raw[10]={0};CStringParser* p=reinterpret_cast<CStringParser*>(raw);char* filename="creature.def";slot(p->Filename)=filename;unsigned long managerRaw[11]={0};CPersistContext* m=reinterpret_cast<CPersistContext*>(managerRaw);View::setup(m);
 CCharString message("bad symbol");p->Line=0;View::report(m,*p,message);if(calls!=1 || strcmp(result,"creature.def(1) : error : bad symbol"))return 1;
 p->Line=41;View::report(m,*p,message);if(calls!=2 || strcmp(result,"creature.def(42) : error : bad symbol"))return 2;

 puts("PERSIST_ERROR_OK cases=2");return 0;}
