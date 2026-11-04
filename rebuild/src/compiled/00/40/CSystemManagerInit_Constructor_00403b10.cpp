#include "fable_system.h"

#include <string.h>

CSystemManagerInit::CSystemManagerInit()
{
    InstanceHandle = 0;
    CommandLine = 0;
    ExclusiveMode = true;
    Flags = 0;
    IsErrorFailureFunc = 0;
    PreferPrimaryDisplayDevice = false;
    memset(Identifier, 0, sizeof(Identifier));
    strncpy(Identifier, "BBB GUID", 8);
    Identifier[8] = '\0';
    LogfileName = L"bbb.log";
    ScratchBufferSize = 0x20000;
    AttachToExternalWindow = false;
    PreventExclusiveModeChanges = false;
    ApplicationWindowHandle = 0;
    UseAccurateTimer = false;
    AppReinitFunc = 0;
    AppReinitContext = 0;
    TempPath = L"C:\\temp";
    IMEMessageFunc = 0;
    MessageFilter = 0;
    WaitWhileInactive = true;
}
