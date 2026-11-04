#include "fable_string.h"
#include <stdio.h>
#include <string.h>
#include <new>
#include "../src/compiled/00/99/CCharString_Destructor_0099eae0.cpp"
#include "../src/compiled/00/99/CCharString_UnassignString_0099e9b0.cpp"
fable_i32 g_CCharStringInstanceCount_013BD800=5;
int main(){
 unsigned char raw[sizeof(CCharString)+8];memset(raw,0xA5,sizeof(raw));
 CCharString* value=new(raw+4)CCharString;
 if(*reinterpret_cast<void**>(value)!=0 || g_CCharStringInstanceCount_013BD800!=6 || raw[3]!=0xA5 || raw[sizeof(raw)-4]!=0xA5)return 1;
 value->~CCharString();if(g_CCharStringInstanceCount_013BD800!=5)return 2;
 puts("CHAR_STRING_DEFAULT_0099e4b0_PASS");return 0;
}
