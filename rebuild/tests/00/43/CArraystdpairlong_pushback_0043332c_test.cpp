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
static int erases,inserts;static CCharString *firstSeen,*lastSeen;static size_t countSeen;static const CCharString* valueSeen;
template<> CCharString* _STL::vector<CCharString>::erase(CCharString* first,CCharString* last){++erases;firstSeen=first;lastSeen=last;return first;}
template<> void _STL::vector<CCharString>::_M_fill_insert(CCharString* pos,size_t n,const CCharString& x){++inserts;firstSeen=pos;countSeen=n;valueSeen=&x;}
int main(){CCharString values[4];CCharString fill("fill");_STL::vector<CCharString> v;v._M_start=values;v._M_finish=values+3;v._M_end_of_storage._M_data=values+4;
 v.resize(1,fill);if(erases!=1||inserts||firstSeen!=values+1||lastSeen!=values+3)return 1;
 v.resize(3,fill);if(inserts!=1||countSeen!=0||firstSeen!=values+3||valueSeen!=&fill)return 2;
 v.resize(6,fill);if(inserts!=2||countSeen!=3||valueSeen!=&fill)return 3;
 v.resize(0,fill);if(erases!=2||firstSeen!=values||lastSeen!=values+3)return 4;
 v._M_start=v._M_finish=v._M_end_of_storage._M_data=0;puts("STRING_RESIZE_PASS");return 0;}

template CCharString* _STL::vector<CCharString>::erase(CCharString*,CCharString*);
