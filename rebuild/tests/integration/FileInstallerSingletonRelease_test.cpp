#include <stdio.h>
#include <stdlib.h>
#include "fable_file_installer.h"
#include "fable_boot.h"
static FableReferenceCount original,replacement;
static unsigned mode,destroyCalls,freeCalls;
static void* expectedObject;
static void* freed;
static void require(bool ok){if(!ok)abort();}
static void FABLE_FASTCALL Destroy(void* object){
    require(object==expectedObject&&original.owners==0);++destroyCalls;
    if(mode==1)g_CFileInstallerSingleton_013CA818.reference=&replacement;
    if(mode==2)g_CFileInstallerSingleton_013CA818.reference=0;
    if(mode==3)g_CFileInstallerSingleton_013CA818.object=(CFileInstaller*)0x12345678;
}
void FABLE_CDECL FableOperatorDelete(void* object){freed=object;++freeCalls;}
int main(){
    const unsigned counts[]={0,1,2,0xffffffff};unsigned cases=0;
    for(unsigned absent=0;absent!=2;++absent)for(unsigned n=0;n!=4;++n)for(mode=0;mode!=4;++mode){
        expectedObject=(void*)(0x80000000+cases);original.owners=counts[n];original.destroy=Destroy;original.object=expectedObject;
        replacement.owners=17;replacement.destroy=Destroy;replacement.object=(void*)0xabcdef;
        g_CFileInstallerSingleton_013CA818.object=(CFileInstaller*)expectedObject;
        g_CFileInstallerSingleton_013CA818.reference=absent?0:&original;
        destroyCalls=freeCalls=0;freed=(void*)0xfedcba;
        CTBaseSingleton<CFileInstaller>::Release();
        bool destroyed=!absent&&counts[n]==1;
        require(destroyCalls==(unsigned)destroyed&&freeCalls==(unsigned)destroyed);
        require(original.owners==(absent?counts[n]:counts[n]-1));
        if(destroyed)require(freed==(mode==1?(void*)&replacement:mode==2?0:(void*)&original));
        require(replacement.owners==17&&replacement.object==(void*)0xabcdef);
        require(!g_CFileInstallerSingleton_013CA818.object&&!g_CFileInstallerSingleton_013CA818.reference);++cases;
    }
    printf("INSTALLER_SINGLETON_RELEASE_PASS cases=%u\n",cases);return 0;
}
