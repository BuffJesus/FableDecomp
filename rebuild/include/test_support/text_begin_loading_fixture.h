#include "fable_game_text_bank.h"
#include <stdio.h>
#include <string.h>
static unsigned long bankBytes[sizeof(NGameText::CDataBank)/4];
static NGameText::CDataBankEntry* entries[4];
static unsigned long textEntry[2],groupEntry[1];
static unsigned long dataType,indexSeen;
static int typeCalls,textCalls,groupCalls;
static const void* entrySeen;
static const NGameText::CDataBank* bankSeen;
unsigned long CBankFile::GetEntryDataType(unsigned long index) const {
    ++typeCalls; indexSeen=index; return dataType;
}
void NGameText::CDataBankTextEntry::BeginLoading(unsigned long index,CDataBank& bank) const {
    ++textCalls; entrySeen=this; bankSeen=&bank; indexSeen=index;
}
void NGameText::CDataBankGroupEntry::BeginLoading(unsigned long index,CDataBank& bank) const {
    ++groupCalls; entrySeen=this; bankSeen=&bank; indexSeen=index;
}
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d\n",__LINE__); return 1; } } while(0)
int main() {
    NGameText::CDataBank& bank=*reinterpret_cast<NGameText::CDataBank*>(bankBytes);
    bank.Entries._M_start=entries;
    entries[2]=reinterpret_cast<NGameText::CDataBankEntry*>(textEntry);
    dataType=1; bank.BeginLoadingText(2); CHECK(textCalls==0 && typeCalls==1 && indexSeen==2);
    dataType=0; entries[2]=0; bank.BeginLoadingText(2); CHECK(textCalls==0 && typeCalls==2);
    entries[2]=reinterpret_cast<NGameText::CDataBankEntry*>(textEntry);
    bank.BeginLoadingText(2); CHECK(textCalls==1 && typeCalls==3 && entrySeen==textEntry && bankSeen==&bank && indexSeen==2);
    entries[3]=reinterpret_cast<NGameText::CDataBankEntry*>(groupEntry);
    bank.BeginLoadingGroup(3); CHECK(groupCalls==0 && typeCalls==4 && indexSeen==3);
    dataType=1; entries[3]=0; bank.BeginLoadingGroup(3); CHECK(groupCalls==0 && typeCalls==5);
    entries[3]=reinterpret_cast<NGameText::CDataBankEntry*>(groupEntry);
    bank.BeginLoadingGroup(3); CHECK(groupCalls==1 && typeCalls==6 && entrySeen==groupEntry && bankSeen==&bank && indexSeen==3);
    puts("TEXT_BEGIN_LOADING_PASS wrongType=2 absentEntry=2 dispatched=2");
    return 0;
}
