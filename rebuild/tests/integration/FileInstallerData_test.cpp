#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fable_file_installer.h"
#include "fable_boot.h"
extern const char g_FileInstallerCompressedExtension_0129B448[];
extern const char g_FileInstallerPlainExtension_0129B440[];
void* FABLE_CDECL FableInstallerContainerAllocate(unsigned long);
int main()
{
    if(g_CFileInstallerSingleton_013CA818.object || g_CFileInstallerSingleton_013CA818.reference)abort();
    if(strcmp(g_FileInstallerCompressedExtension_0129B448,".biz") || strcmp(g_FileInstallerPlainExtension_0129B440,".big"))abort();
    for(unsigned long size=1;size!=257;++size){
        void* block=FableInstallerContainerAllocate(size);if(!block)abort();
        memset(block,0xa5,size);free(block);
        block=FableFileInstallerAllocate(size);if(!block)abort();
        memset(block,0x5a,size);FableOperatorDelete(block);
        block=FableOperatorNew(size);if(!block)abort();
        memset(block,0x3c,size);FableOperatorDelete(block);
    }
    puts("INSTALLER_DATA_PASS allocations=768");return 0;
}
