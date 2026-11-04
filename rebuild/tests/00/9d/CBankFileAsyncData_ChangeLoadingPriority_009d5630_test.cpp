#include "fable_threaded_file.h"
#include "fable_game_text_bank.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static CThreadedFile* expectedFile;
static unsigned long expectedUID,expectedPriority,calls;
void CThreadedFile::SetObjectPriority(unsigned long uid,EThreadedFilePriority priority)
{
    if (this!=expectedFile || uid!=expectedUID || static_cast<unsigned long>(priority)!=expectedPriority)
        exit(81);
    ++calls;
}

int main()
{
    const unsigned long uids[]={0,1,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};
    const unsigned long priorities[]={0,1,2,3,4,5,6,7,0xffffffff};
    unsigned cases=0;
    for(unsigned seed=0;seed<4;++seed)for(unsigned u=0;u<6;++u)
    for(unsigned p=0;p<9;++p)for(unsigned ordinal=0;ordinal<4;++ordinal) {
        unsigned long dataWords[40],bankWords[(sizeof(CBankFileAsync)+32)/4],fileWords[24];
        memset(dataWords,seed*51,sizeof(dataWords));memset(bankWords,seed*37,sizeof(bankWords));memset(fileWords,seed*19,sizeof(fileWords));
        CBankFileAsyncData* data=reinterpret_cast<CBankFileAsyncData*>(dataWords+4+ordinal);
        CBankFileAsync* bank=reinterpret_cast<CBankFileAsync*>(bankWords+4);
        expectedFile=reinterpret_cast<CThreadedFile*>(fileWords+ordinal);
        expectedUID=uids[u];expectedPriority=priorities[p];calls=0;
        data->ThreadedFileUID=expectedUID;
        // Populate the reference storage without constructing bank runtime services.
        *reinterpret_cast<CBankFileAsync**>(reinterpret_cast<char*>(data)+0x28)=expectedUID?bank:0;
        bank->ThreadedFile.data_=expectedFile;
        unsigned long dataBefore[40],bankBefore[(sizeof(CBankFileAsync)+32)/4],fileBefore[24];
        memcpy(dataBefore,dataWords,sizeof(dataWords));memcpy(bankBefore,bankWords,sizeof(bankWords));memcpy(fileBefore,fileWords,sizeof(fileWords));
        data->ChangeLoadingPriority(static_cast<EThreadedFilePriority>(expectedPriority));
        if(calls!=(expectedUID?1UL:0UL) || memcmp(dataBefore,dataWords,sizeof(dataWords)) || memcmp(bankBefore,bankWords,sizeof(bankWords)) || memcmp(fileBefore,fileWords,sizeof(fileWords)))return 82;
        ++cases;
    }
    printf("NATIVE_BANK_PRIORITY_HOST_PASS cases=%u\n",cases);return cases==864?0:83;
}
