#pragma once
#include "rebuild_abi.h"
// Retail SHGetFolderPathW import slot01440280, already named by the
// My Documents provider. Shared with the writable engine cache provider.
typedef long (FABLE_STDCALL *FableGetFolderPathW_0041A430)(
    void* window,int folder,void* token,fable_u32 flags,wchar_t* path);
extern "C" FableGetFolderPathW_0041A430 g_FableGetFolderPathW_0041A430;
