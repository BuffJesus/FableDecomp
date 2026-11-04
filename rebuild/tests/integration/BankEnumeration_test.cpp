#include "fable_font_data_banks.h"
#include <stdio.h>
#include <string.h>
struct Ref{const char* text;unsigned long owners;};static Ref literal;static int copies,constructed,destroyed;static const CCharString* source;static const char* literalText;
char g_FableEmptyNarrowString_0129AAF4[]="";
CCharString::CCharString(const CCharString& x){storage_=x.storage_;if(storage_)++((Ref*)storage_)->owners;source=&x;++copies;}
CCharString::CCharString(const char* text,long len){if(len!=-1)constructed=-100;literal.text=text;literal.owners=1;storage_=(CCharStringData*)&literal;literalText=text;++constructed;}
CCharString::~CCharString(){if(storage_)--((Ref*)storage_)->owners;++destroyed;}
struct BaseAccess:CBankFile{static CCharString Call(const CBankFile* p,unsigned long t){return ((const BaseAccess*)p)->CBankFile::GetEnumerationType(t);}};
struct FontAccess:CFontDataBank{static CCharString Call(const CFontDataBank* p,unsigned long t){return ((const FontAccess*)p)->CFontDataBank::GetEnumerationType(t);}};
struct StreamAccess:CStreamingFontDataBank{static CCharString Call(const CStreamingFontDataBank* p,unsigned long t){return ((const StreamAccess*)p)->CStreamingFontDataBank::GetEnumerationType(t);}};
struct TextAccess:NGameText::CDataBank{static CCharString Call(const NGameText::CDataBank* p,unsigned long t){return ((const TextAccess*)p)->NGameText::CDataBank::GetEnumerationType(t);}};
static void check(const CCharString& out,CBankFile* bank,Ref* ref,unsigned long before,unsigned long type,int kind,int& failed){
 if(kind==3){const char* expected=type==0?"EGameText":type==1?"ETextGroup":"";if(copies||constructed!=1||strcmp(out.GetConstCharPtr(),expected)||strcmp(literalText,expected)||literal.owners!=1)failed=1;}
 else if(copies!=1||constructed||source!=&bank->HeaderEnumerationType||*(Ref* const*)&out!=ref||(ref&&ref->owners!=before+1))failed=2;
}
int main(){static const unsigned long types[]={0,1,2,3,127,255,0x80000000,0xffffffff},counts[]={0,1,7,0xffffffff};static const char* texts[]={0,"","FontSymbols","\x80\xff","long-enumeration-name"};int cases=0;
 for(int kind=0;kind<4;++kind)for(int t=0;t<8;++t)for(int x=0;x<5;++x)for(int c=0;c<4;++c){union{double align;char data[0x214];} mem;memset(&mem,0xa5,sizeof(mem));CBankFile* bank=(CBankFile*)&mem;Ref input={texts[x],counts[c]};Ref* ref=x?&input:0;*(Ref**)&bank->HeaderEnumerationType=ref;copies=constructed=destroyed=0;source=0;literalText=0;int failed=0;
  if(kind==0){CCharString out=BaseAccess::Call(bank,types[t]);check(out,bank,ref,counts[c],types[t],kind,failed);}
  else if(kind==1){CCharString out=FontAccess::Call((CFontDataBank*)bank,types[t]);check(out,bank,ref,counts[c],types[t],kind,failed);}
  else if(kind==2){CCharString out=StreamAccess::Call((CStreamingFontDataBank*)bank,types[t]);check(out,bank,ref,counts[c],types[t],kind,failed);}
  else {CCharString out=TextAccess::Call((NGameText::CDataBank*)bank,types[t]);check(out,bank,ref,counts[c],types[t],kind,failed);}
  if(failed||destroyed!=1||input.owners!=counts[c]||(kind==3&&literal.owners!=0))return 1;++cases;
 }
 printf("BANK_ENUMERATION_BEHAVIOR_PASS scenarios=%d\n",cases);return 0;}
