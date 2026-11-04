// Verified native file-installer ownership provider.
#include "fable_file_installer.h"
#include "fable_boot.h"
void FABLE_FASTCALL FableDestroyFileInstaller_00403630(void* object);
void CCountedFileInstaller::operator=(CFileInstaller* value)
{
    if(reference){
        --static_cast<FableReferenceCount*>(reference)->owners;
        if(static_cast<FableReferenceCount*>(reference)->owners==0){
            static_cast<FableReferenceCount*>(reference)->destroy(static_cast<FableReferenceCount*>(reference)->object);
            FableOperatorDelete(reference);
        }
    }
    reference=0;
    object=value;
    if(value){
        FableReferenceCount* block=static_cast<FableReferenceCount*>(FableOperatorNew(sizeof(FableReferenceCount)));
        FableReferenceCount* result;
        if(block){block->owners=1;block->destroy=FableDestroyFileInstaller_00403630;block->object=object;result=block;}
        else result=0;
        reference=result;
    }
}
