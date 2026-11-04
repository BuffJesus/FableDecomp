// Production-linked SetupRead ABI probe. Five child boundaries are controlled.
#include "../../../include/fable_threaded_file_object.h"
#include "../../../include/fable_bank_async_entry.h"
#include <stdio.h>
#include <string.h>
extern "C" void __fastcall FixtureSetupRead(CBankFileAsyncEntry*,void*,unsigned long,CBankFileAsyncData&,unsigned long,unsigned long,unsigned long,EThreadedFilePriority);
#pragma comment(linker,"/alternatename:@FixtureSetupRead@32=?SetupRead@CBankFileAsyncEntry@@AAEXKAAVCBankFileAsyncData@@KKKW4EThreadedFilePriority@@@Z")
static CBankFileAsync* entryBank;
static CBankFileAsyncData* activeData;
static CThreadedFile* activeFile;
static void* currentBuffer;
static unsigned long wantedSize,wantedOffset,wantedUID,steps;
static bool bad,syncMode,noncachedMode;
static unsigned long BaseOffset() {unsigned long base=0xfffffff7UL;return base+3*17;}
static void Step(unsigned role) {steps=steps*10+role;}
bool CBankFileAsyncData::SetupBuffer(bool force) {
    Step(1);if(this!=activeData || !force)bad=true;Buffer=currentBuffer;return false;
}
unsigned long CBankFile::GetEntryDataOffset(unsigned long index) const {
    Step(2);if(this!=entryBank || index!=3)bad=true;return BaseOffset();
}
unsigned long CBankFile::Align(unsigned long size) const {
    Step(3);if(this!=entryBank || size!=wantedSize)bad=true;return (size+31)&~31UL;
}
void CBankFile::ReadData(unsigned long offset,void* buffer,unsigned long size) const {
    Step(4);if(this!=entryBank || offset!=BaseOffset()+wantedOffset || buffer!=currentBuffer || size!=wantedSize)bad=true;
    activeData->Finished=false;
}
unsigned long CThreadedFile::AddObject(const CThreadedFileObjectDesc& desc) {
    Step(5);if(this!=activeFile || &desc!=&activeData->LoadDesc)bad=true;return wantedUID;
}
int main() {
    const unsigned long uids[]={0,0x80000000UL,0xffffffffUL};
    unsigned cases=0;
    for(unsigned mode=0;mode<4;++mode)for(unsigned n=0;n<3;++n)for(unsigned sizeCase=0;sizeCase<2;++sizeCase) {
        unsigned long entryWords[12],dataWords[(sizeof(CBankFileAsyncData)+32)/4];
        unsigned long bankWords[(sizeof(CBankFileAsync)+32)/4],otherWords[(sizeof(CBankFileAsync)+32)/4],fileWords[32];
        memset(entryWords,0x37,sizeof(entryWords));memset(dataWords,0x59,sizeof(dataWords));
        memset(bankWords,0x71,sizeof(bankWords));memset(otherWords,0x83,sizeof(otherWords));memset(fileWords,0x97,sizeof(fileWords));
        CBankFileAsyncEntry* entry=reinterpret_cast<CBankFileAsyncEntry*>(entryWords+4);
        activeData=reinterpret_cast<CBankFileAsyncData*>(dataWords+4);entryBank=reinterpret_cast<CBankFileAsync*>(bankWords+4);
        CBankFileAsync* other=reinterpret_cast<CBankFileAsync*>(otherWords+4);activeFile=reinterpret_cast<CThreadedFile*>(fileWords+4);
        *reinterpret_cast<CBankFileAsync**>(reinterpret_cast<char*>(entry)+4)=entryBank;entry->Index=3;
        *reinterpret_cast<CBankFileAsync**>(reinterpret_cast<char*>(activeData)+0x28)=other;
        *reinterpret_cast<CThreadedFile**>(reinterpret_cast<char*>(other)+0x110)=activeFile;
        syncMode=(mode&2)!=0;noncachedMode=(mode&1)!=0;entryBank->DisableAsync=syncMode;entryBank->NonCached=noncachedMode;
        currentBuffer=fileWords+20;activeData->Buffer=syncMode?fileWords+16:currentBuffer;
        wantedSize=sizeCase?0xffffffffUL:17;wantedOffset=sizeCase?0xfffffff0UL:0;wantedUID=uids[n];bad=false;steps=0;
        unsigned char expectedData[sizeof(dataWords)],beforeBank[sizeof(bankWords)],beforeOther[sizeof(otherWords)],beforeEntry[sizeof(entryWords)],beforeFile[sizeof(fileWords)];
        memcpy(expectedData,dataWords,sizeof(dataWords));memcpy(beforeBank,bankWords,sizeof(bankWords));memcpy(beforeOther,otherWords,sizeof(otherWords));
        memcpy(beforeEntry,entryWords,sizeof(entryWords));memcpy(beforeFile,fileWords,sizeof(fileWords));
        CBankFileAsyncData* expected=reinterpret_cast<CBankFileAsyncData*>(expectedData+16);
        if(syncMode) {expected->Buffer=currentBuffer;expected->Finished=true;}
        else {
            expected->LoadDesc.PDst=currentBuffer;expected->LoadDesc.FilePos=BaseOffset()+wantedOffset;
            expected->LoadDesc.Size=noncachedMode?((wantedSize+31)&~31UL):wantedSize;
            expected->LoadDesc.PCallback=activeData;expected->LoadDesc.Priority=static_cast<EThreadedFilePriority>(6);
            expected->LoadDesc.QueueWhenFinished=false;expected->ThreadedFileUID=wantedUID;
        }
        FixtureSetupRead(entry,0,wantedOffset,*activeData,wantedSize,0xdeadbeefUL,0x87654321UL,static_cast<EThreadedFilePriority>(6));
        if(bad || steps!=(syncMode?124:noncachedMode?235:25) || memcmp(expectedData,dataWords,sizeof(dataWords)) ||
           memcmp(beforeBank,bankWords,sizeof(bankWords)) || memcmp(beforeOther,otherWords,sizeof(otherWords)) ||
           memcmp(beforeEntry,entryWords,sizeof(entryWords)) || memcmp(beforeFile,fileWords,sizeof(fileWords))) return 1;
        ++cases;
    }
    printf("SETUPREAD_TYPED_PASS cases=%u\n",cases);return 0;
}
