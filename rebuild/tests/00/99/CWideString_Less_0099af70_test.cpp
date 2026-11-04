#include "fable_string.h"
#include <stdio.h>
#include <string.h>
wchar_t g_FableEmptyWideString_0129A8E0[1]={0};
int main() {
    wchar_t text[5][4]={{L'z',0},{L'a',0},{0},{0x8000,0},{L'a',0,L'z',0}};
    struct Storage {const wchar_t* text;unsigned long guard[3];} data[5];
    unsigned char objects[6][sizeof(CWideString)];memset(objects,0,sizeof(objects));
    for(unsigned i=0;i<5;++i){data[i].text=text[i];data[i].guard[0]=data[i].guard[1]=data[i].guard[2]=0xA5A5A5A5;*reinterpret_cast<void**>(objects[i])=&data[i];}
    CWideString* z=reinterpret_cast<CWideString*>(objects[0]);CWideString* a=reinterpret_cast<CWideString*>(objects[1]);
    CWideString* empty=reinterpret_cast<CWideString*>(objects[2]);CWideString* high=reinterpret_cast<CWideString*>(objects[3]);
    CWideString* embedded=reinterpret_cast<CWideString*>(objects[4]);CWideString* none=reinterpret_cast<CWideString*>(objects[5]);
    if(*z<*a || !(*a<*z) || *a<*a || *none<*empty || *empty<*none || *none<*none)return 1;
    if(!(*none<*a) || *a<*none || !(*a<*high) || *high<*a || *a<*embedded || *embedded<*a)return 2;
    for(unsigned j=0;j<5;++j)if(data[j].text!=text[j] || data[j].guard[0]!=0xA5A5A5A5 || data[j].guard[1]!=0xA5A5A5A5 || data[j].guard[2]!=0xA5A5A5A5 || *reinterpret_cast<void**>(objects[j])!=&data[j])return 3;
    puts("WIDE_STRING_LESS_0099af70_PASS");return 0;
}
