#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_stlport_char_string_vector.h"
static int live, reads, resizes, overflows, bad;
char g_FableEmptyNarrowString_0129AAF4[1] = {0};
CCharString::CCharString() : storage_(0) { ++live; }
CCharString::CCharString(const char* text, long length) : storage_(0) {
 ++live; if(length < 0)length=(long)strlen(text);
 storage_=(CCharStringData*)malloc(sizeof(CCharStringData));
 storage_->Data.PData=(char*)malloc(length+1);memcpy(storage_->Data.PData,text,length);storage_->Data.PData[length]=0;
 storage_->Data.StringLength=length;storage_->owners=1;
}
CCharString::CCharString(const CCharString& x) : storage_(x.storage_) {++live;if(storage_)++storage_->owners;}
void CCharString::UnassignString(){if(storage_ && --storage_->owners==0){free(storage_->Data.PData);free(storage_);}storage_=0;}
CCharString::~CCharString(){UnassignString();--live;}
CCharString& CCharString::operator=(const CCharString& x){if(this==&x)return *this;if(x.storage_)++x.storage_->owners;UnassignString();storage_=x.storage_;return *this;}
static CCharString *destroyFirst,*destroyLast;
template<> void FABLE_FASTCALL _STL::_Destroy_range<CCharString>(CCharString* first,CCharString* last){destroyFirst=first;destroyLast=last;for(;first!=last;++first)first->~CCharString();}
int main(){for(int mode=0;mode<3;++mode){
 unsigned long raw[4];CCharString* a=(CCharString*)raw;const char* words[]={"one","two","three","four"};
 for(int i=0;i<4;++i)new(a+i)CCharString(words[i]);
 _STL::vector<CCharString> v;v._M_start=a;v._M_finish=a+4;v._M_end_of_storage._M_data=a+4;
 int first=mode==0?1:0,last=mode==0?3:mode==1?4:0;CCharString* result=v.erase(a+first,a+last);
 int left=4-(last-first);if(result!=a+first||v.size()!=left||destroyFirst!=a+left||destroyLast!=a+4||live!=left)return 1;
 if(mode==0&&(strcmp(a[0].GetConstCharPtr(),"one")||strcmp(a[1].GetConstCharPtr(),"four")))return 2;
 if(mode==2)for(i=0;i<4;++i)if(strcmp(a[i].GetConstCharPtr(),words[i]))return 3;
 for(i=0;i<left;++i)a[i].~CCharString();v._M_start=v._M_finish=v._M_end_of_storage._M_data=0;if(live)return 4;
 }puts("STRING_ERASE_PASS");return 0;}