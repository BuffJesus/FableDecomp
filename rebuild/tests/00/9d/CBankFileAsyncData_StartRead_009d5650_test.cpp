// Links the production StartRead TU; the file submission boundary is controlled.
#include "../../../include/fable_threaded_file_object.h"
#include <stdio.h>
#include <string.h>
extern "C" void __fastcall FixtureStartRead(CBankFileAsyncData*,void*);
#pragma comment(linker,"/alternatename:@FixtureStartRead@8=?StartRead@CBankFileAsyncData@@AAEXXZ")
static CThreadedFile* expectedFile;
static const CThreadedFileObjectDesc* expectedDesc;
static unsigned long nextUID,calls;
static bool bad;
unsigned long CThreadedFile::AddObject(const CThreadedFileObjectDesc& desc) {
    if(this!=expectedFile || &desc!=expectedDesc) bad=true;
    ++calls;return nextUID;
}
int main() {
    const unsigned long uids[]={0,1,0x7fffffffUL,0x80000000UL,0xfffffffeUL,0xffffffffUL};
    for(unsigned seed=0;seed<4;++seed) for(unsigned n=0;n<6;++n) {
        unsigned long dataWords[(sizeof(CBankFileAsyncData)+32)/4];
        unsigned long bankWords[(sizeof(CBankFileAsync)+32)/4];
        unsigned long fileWords[32];
        memset(dataWords,0x37+seed,sizeof(dataWords));
        memset(bankWords,0x51+seed,sizeof(bankWords));memset(fileWords,0x93+seed,sizeof(fileWords));
        CBankFileAsyncData* data=reinterpret_cast<CBankFileAsyncData*>(dataWords+4);
        CBankFileAsync* bank=reinterpret_cast<CBankFileAsync*>(bankWords+4);
        expectedFile=reinterpret_cast<CThreadedFile*>(fileWords+4);expectedDesc=&data->LoadDesc;
        *reinterpret_cast<CBankFileAsync**>(reinterpret_cast<unsigned char*>(data)+0x28)=bank;
        *reinterpret_cast<CThreadedFile**>(reinterpret_cast<unsigned char*>(bank)+0x110)=expectedFile;
        unsigned char beforeData[sizeof(dataWords)],beforeBank[sizeof(bankWords)],beforeFile[sizeof(fileWords)];
        memcpy(beforeData,dataWords,sizeof(dataWords));memcpy(beforeBank,bankWords,sizeof(bankWords));
        memcpy(beforeFile,fileWords,sizeof(fileWords));nextUID=uids[n];calls=0;bad=false;
        FixtureStartRead(data,0);memcpy(beforeData+16+0x18,&nextUID,4);
        if(bad || calls!=1 || memcmp(beforeData,dataWords,sizeof(dataWords)) ||
            memcmp(beforeBank,bankWords,sizeof(bankWords)) || memcmp(beforeFile,fileWords,sizeof(fileWords))) return 1;
    }
    puts("STARTREAD_OK typed_cases=24");return 0;
}
