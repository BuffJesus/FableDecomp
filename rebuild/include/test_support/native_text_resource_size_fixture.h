#pragma comment(lib, "kernel32.lib")
#include "fable_game_text_bank.h"
#include <stdio.h>
#include <string.h>
static int asyncCalls;
static unsigned long asyncBytes[32];

#define CHECK(x) do { if (!(x)) { printf("FAIL line %d\n",__LINE__); return 1; } } while(0)
int main() {
    unsigned long textBytes[sizeof(NGameText::CTextEntry)/4];
    unsigned long groupBytes[sizeof(NGameText::CGroupEntry)/4];
    memset(textBytes,0,sizeof(textBytes)); memset(groupBytes,0,sizeof(groupBytes));
    NGameText::CTextEntry& text = *reinterpret_cast<NGameText::CTextEntry*>(textBytes);
    NGameText::CGroupEntry& group = *reinterpret_cast<NGameText::CGroupEntry*>(groupBytes);
    CHECK(text.Sizeof()==68 && group.Sizeof()==52);
    unsigned long charBytes[5]={0,0,0x80000028,0,0};
    *reinterpret_cast<CCharStringData**>(&text.SpeechBank)=reinterpret_cast<CCharStringData*>(charBytes);
    CHECK(text.SpeechBank.Sizeof()==61 && text.Sizeof()==125);
    charBytes[2]=0;
    CHECK(text.SpeechBank.Sizeof()==21 && text.Sizeof()==85);
    wchar_t letters[20];
    unsigned long wideBytes[4]={0,0,0,0};
    CWideStringData& wide = *reinterpret_cast<CWideStringData*>(wideBytes);
    wide.Data._M_start=letters; wide.Data._M_finish=letters+7; wide.Data._M_end_of_storage._M_data=letters+20;
    *reinterpret_cast<CWideStringData**>(&text.Text)=&wide;
    CHECK(text.Text.Sizeof()==18 && text.Sizeof()==99);
    unsigned long tags[10];
    text.Tags._M_start=reinterpret_cast<NGameText::CTextTag*>(tags);
    text.Tags._M_finish=text.Tags._M_start+2;
    text.Tags._M_end_of_storage._M_data=text.Tags._M_start+5;
    CHECK(text.Sizeof()==139);
    long entries[9];
    group.Entries._M_start=entries; group.Entries._M_finish=entries+1;
    group.Entries._M_end_of_storage._M_data=entries+9;
    CHECK(group.Sizeof()==88);
    reinterpret_cast<CBankFileAsyncData*>(asyncBytes)->InternalAllocatedBufferSize=96;
    text.PFileData.Data=reinterpret_cast<CBankFileAsyncData*>(asyncBytes);
    group.PFileData.Data=text.PFileData.Data;
    CHECK(text.Sizeof()==307 && group.Sizeof()==256);
    puts("NATIVE_RESOURCE_SIZE_GROUP_PASS empty=2 narrowMask=1 wideLength=1 vectorCapacity=2 asyncPayload=2");
    return 0;
}

#include "../../src/compiled/00/99/String_Sizeof_0099e500.cpp"

#include "../../src/compiled/00/9d/CBankFileAsyncData_Accessors_009d54e0.cpp"

#include "../../src/compiled/00/a6/CCriticalSectionLock_Constructor_00a62920.cpp"

#include "../../src/compiled/00/a6/CCriticalSectionLock_Destructor_00a629a0.cpp"
