#include "fable_file_installer.h"
void FABLE_FASTCALL FableDestroyFileInstaller_00403630(void* object)
{
    delete static_cast<CFileInstaller*>(object);
}
