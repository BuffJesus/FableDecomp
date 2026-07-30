#include "fable_resource_directories.h"

CWideString FABLE_FASTCALL
FableGetAutoSaveCheckpointDirectory_0041A4C0()
{
    return FableGetMyDocumentsDirectory_0041A430() +
        L"My Games\\Fable\\";
}
