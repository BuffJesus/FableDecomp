#pragma once
// Fixture-only reference-counted strings. No vector candidate body is defined here.
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_stlport_char_string_vector.h"
static int live, dataLive, copies, deaths;
char g_FableEmptyNarrowString_0129AAF4[1] = {0};
CCharString::CCharString() : storage_(0) { ++live; }
CCharString::CCharString(const char* text, long length) : storage_(0) {
 ++live; ++dataLive; if(length < 0)length=(long)strlen(text);
 storage_=(CCharStringData*)malloc(sizeof(CCharStringData));
 storage_->Data.PData=(char*)malloc(length+1);memcpy(storage_->Data.PData,text,length);storage_->Data.PData[length]=0;
 storage_->Data.StringLength=length;storage_->owners=1;
}
CCharString::CCharString(const CCharString& x) : storage_(x.storage_) {++live;++copies;if(storage_)++storage_->owners;}
void CCharString::UnassignString(){if(storage_ && --storage_->owners==0){free(storage_->Data.PData);free(storage_);--dataLive;}storage_=0;}
CCharString::~CCharString(){UnassignString();--live;++deaths;}
CCharString& CCharString::operator=(const CCharString& x){if(this==&x)return *this;if(x.storage_)++x.storage_->owners;UnassignString();storage_=x.storage_;return *this;}
