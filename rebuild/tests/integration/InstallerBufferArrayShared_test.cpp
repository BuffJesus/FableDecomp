#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_file_installer.h"
static CFileInstallerBufferArray* owner;
static InstallerBufferChunk *expectedFirst,*expectedLast;
static unsigned calls,phase;
static void require(bool ok) { if(!ok)abort(); }
void FABLE_FASTCALL FableInstallerDestroyBufferRange(InstallerBufferChunk* first,InstallerBufferChunk* last,void* token)
{
    require(token!=0 && first==expectedFirst && last==expectedLast);
    if(phase==1 && calls==1)require(owner->first==0 && owner->last==0 && owner->capacity==0);
    ++calls;
}
int main()
{
    unsigned cases=0;
    for(unsigned length=0;length!=8;++length)for(unsigned begin=0;begin<=length;++begin)for(unsigned end=begin;end<=length;++end)
    {
        InstallerBufferChunk actual[10],expected[10];
        for(unsigned i=0;i!=10;++i){actual[i].allocation=(void*)(0x1000+i*16);actual[i].unknown04=0xa000+i;}
        memcpy(expected,actual,sizeof(actual));
        for(i=end;i!=length;++i)expected[begin+i-end]=expected[i];
        CFileInstallerBufferArray array;owner=&array;array.first=actual;array.last=actual+length;array.capacity=actual+10;
        expectedFirst=actual+length-(end-begin);expectedLast=actual+length;calls=phase=0;
        require(array.RemoveRange(actual+begin,actual+end)==actual+begin);
        require(calls==1 && array.last==expectedFirst && array.first==actual && array.capacity==actual+10 && !memcmp(actual,expected,sizeof(actual)));
        ++cases;
    }
    for(unsigned capacity=0;capacity!=8;++capacity)
    {
        CFileInstallerBufferArray array;owner=&array;
        array.first=capacity?(InstallerBufferChunk*)malloc(capacity*8):0;require(!capacity||array.first!=0);
        array.last=array.first;array.capacity=capacity?array.first+capacity:0;
        expectedFirst=expectedLast=array.first;calls=0;phase=1;
        array.DeallocateMemory();
        require(calls==(capacity?2:0) && !array.first && !array.last && !array.capacity);++cases;
    }
    printf("INSTALLER_BUFFER_ARRAY_OK cases=%u\n",cases);
    return 0;
}
