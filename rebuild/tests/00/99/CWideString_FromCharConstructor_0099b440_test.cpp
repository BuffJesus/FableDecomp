#include "fable_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/compiled/00/99/STLWideString_FillConstructor_0099ca70.cpp"
#include "../src/compiled/00/99/CWideString_CreateFromCharString_0099b6a0.cpp"
fable_i32 g_CWideStringInstanceCount_013BCA20=10;
char g_FableEmptyNarrowString_0129AAF4[1]={0};
template<> void _STL::_String_base<wchar_t,_STL::allocator<wchar_t> >::_M_throw_length_error() const {abort();}
CWideString::~CWideString(){if(storage_){free(storage_->text);delete storage_;}--g_CWideStringInstanceCount_013BCA20;}
int main(){
 char source[5]={'A',0,static_cast<char>(0x80),static_cast<char>(0xff),0};
 for(unsigned mode=0;mode<3;++mode){
  unsigned char handle[sizeof(CCharString)]={0},raw[sizeof(CCharStringData)]={0};CCharString& input=*reinterpret_cast<CCharString*>(handle);
  CCharStringData* data=reinterpret_cast<CCharStringData*>(raw);data->text=source;data->Data.StringLength=mode==2?4:0;data->owners=7;
  if(mode)*reinterpret_cast<CCharStringData**>(&input)=data;
  unsigned char before[sizeof(raw)];memcpy(before,raw,sizeof(raw));
  {CWideString output=CWideString::CreateFromCharString(input);CWideStringData* result=*reinterpret_cast<CWideStringData**>(&output);
   if(!result || result->owners!=1 || result->Data.size()!=data->Data.StringLength || g_CWideStringInstanceCount_013BCA20!=11)return 1;
   for(unsigned i=0;i<result->Data.size();++i)if(result->text[i]!=static_cast<unsigned char>(source[i]))return 2;
   if(result->text[result->Data.size()]!=0 || memcmp(before,raw,sizeof(raw)))return 3;
  }
  if(g_CWideStringInstanceCount_013BCA20!=10)return 4;
 }
 puts("WIDE_STRING_FROM_CHAR_0099b440_PASS");return 0;
}
