#include "fable_string.h"
#include <stdio.h>
#include <string.h>
char g_FableEmptyNarrowString_0129AAF4[1]={0};
int main(){
 unsigned char raw[sizeof(CCharString)]={0},storage[sizeof(CCharStringData)]={0};CCharString& text=*reinterpret_cast<CCharString*>(raw);
 if(static_cast<const char*>(text)!=g_FableEmptyNarrowString_0129AAF4)return 1;
 CCharStringData* data=reinterpret_cast<CCharStringData*>(storage);char bytes[4]={'a',0,'b',0};data->text=bytes;data->owners=7;*reinterpret_cast<CCharStringData**>(&text)=data;
 unsigned char before[sizeof(storage)];memcpy(before,storage,sizeof(storage));
 if(static_cast<const char*>(text)!=bytes || memcmp(before,storage,sizeof(storage)))return 2;
 bytes[0]=0;if(static_cast<const char*>(text)!=bytes)return 3;
 puts("CHAR_STRING_POINTER_0099e4c0_PASS");return 0;
}
