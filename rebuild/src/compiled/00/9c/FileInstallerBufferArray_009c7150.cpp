// Native cleanup provider: retail parity and behavior evidence enforced by permanent gates.
#include <stdlib.h>
#include "fable_file_installer.h"
InstallerBufferChunk* CFileInstallerBufferArray::RemoveRange(InstallerBufferChunk* begin,InstallerBufferChunk* end)
{
    InstallerBufferChunk* output=begin;
    long remaining=last-end;
    const long sourceOffset=reinterpret_cast<char*>(end)-reinterpret_cast<char*>(begin);
    while(remaining>0) {
        *output=*reinterpret_cast<InstallerBufferChunk*>(reinterpret_cast<char*>(output)+sourceOffset);
        ++output; --remaining;
    }
    unsigned char allocator;
    FableInstallerDestroyBufferRange(output,last,&allocator);
    last=output;
    return begin;
}
void CFileInstallerBufferArray::DeallocateMemory()
{
    if(static_cast<unsigned long>(capacity-first)>0) {
        RemoveRange(first,last);
        InstallerBufferChunk* old=first;
        first=0;
        InstallerBufferChunk* end=last;
        last=0;
        unsigned char allocator;
        capacity=0;
        FableInstallerDestroyBufferRange(old,end,&allocator);
        if(old)free(old);
    }
}
