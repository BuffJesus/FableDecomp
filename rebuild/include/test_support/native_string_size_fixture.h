#include "fable_string.h"
#include <stdio.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d\n",__LINE__); return 1; } } while(0)
int main() {
    unsigned long narrowWord=0,wideWord=0;
    CCharString& narrow=*reinterpret_cast<CCharString*>(&narrowWord);
    CWideString& wide=*reinterpret_cast<CWideString*>(&wideWord);
    CHECK(narrow.Sizeof()==4 && wide.Sizeof()==4);
    unsigned long data[5]={0,999,0,0,0}; narrowWord=reinterpret_cast<unsigned long>(data);
    CHECK(narrow.Sizeof()==21);
    data[2]=0x80000028; CHECK(narrow.Sizeof()==61);
    data[2]=0xffffffff; CHECK(narrow.Sizeof()==0x80000014);
    wchar_t chars[32]; unsigned long wdata[4]={0,0,0,0};
    CWideStringData& storage=*reinterpret_cast<CWideStringData*>(wdata);
    wideWord=reinterpret_cast<unsigned long>(wdata);
    storage.Data._M_start=chars; storage.Data._M_finish=chars; storage.Data._M_end_of_storage._M_data=chars+32;
    CHECK(wide.Sizeof()==4);
    storage.Data._M_finish=chars+7; CHECK(wide.Sizeof()==18);
    puts("NATIVE_STRING_SIZE_PASS null=2 packedCapacity=3 wideLength=2"); return 0;
}
