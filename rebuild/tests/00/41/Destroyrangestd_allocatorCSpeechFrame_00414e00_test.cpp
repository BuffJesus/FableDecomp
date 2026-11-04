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
int main(){unsigned long raw[3];CCharString* a=(CCharString*)raw;for(int i=0;i<3;++i)new(a+i)CCharString("owned");
 _STL::_Destroy_range(a,a);if(live!=3)return 1;
 _STL::_Destroy_range(a+1,a+3);if(live!=1)return 2;
 _STL::_Destroy_range(a,a+1);if(live)return 3;
 puts("STRING_DESTROY_PASS");return 0;}
