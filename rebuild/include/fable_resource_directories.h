#pragma once

#include "fable_string.h"

CWideString FABLE_FASTCALL
FableGetMiscDirectoryA_0041A0A0();
CWideString FABLE_FASTCALL
FableGetMiscDirectoryB_0041A0C0();
CWideString FABLE_FASTCALL
FableGetGraphicsDirectory_0041A100();
CWideString FABLE_FASTCALL
FableGetLanguageDirectoryA_0041A120();
CWideString FABLE_FASTCALL
FableGetShadersDirectory_0041A1E0();
CWideString FABLE_FASTCALL
FableGetCacheDirectory_0041A180();
CWideString FABLE_FASTCALL
FableGetLanguageDirectoryB_0041A330();
CWideString FABLE_FASTCALL
FableGetMyDocumentsDirectory_0041A430();
CWideString FABLE_FASTCALL
FableGetAutoSaveCheckpointDirectory_0041A4C0();

namespace NResourceDirectoryNames
{
    CWideString FABLE_FASTCALL GetMyDocumentsDir();
    CWideString FABLE_FASTCALL GetSaveDir();
}
