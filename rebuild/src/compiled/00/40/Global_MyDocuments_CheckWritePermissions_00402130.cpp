#include <stdio.h>

#include "fable_write_permissions.h"

typedef void* (__stdcall* FableCreateFileW)(
    const wchar_t* fileName,
    unsigned long desiredAccess,
    unsigned long shareMode,
    void* securityAttributes,
    unsigned long creationDisposition,
    unsigned long flagsAndAttributes,
    void* templateFile);
typedef int (__stdcall* FableCloseHandle)(void* handle);
typedef int (__stdcall* FableDeleteFileW)(const wchar_t* fileName);

// kernel32 import slots.
extern "C" FableCreateFileW g_FableCreateFileW_0143FE2C;
extern "C" FableCloseHandle g_FableCloseHandle_0143FE30;
extern "C" FableDeleteFileW g_FableDeleteFileW_0143FE34;

// GFMain's My Documents write-permission probe @ 0x00402130. Returns 0 when
// the folder is local and writable, else the CRC of the error text's symbol.
extern "C" fable_u32 FABLE_FASTCALL
FableCheckMyDocumentsWritePermissions_00402130(
    const CWideString& myDocumentsPath)
{
    CCharString error;
    if (myDocumentsPath.GetSubString(0, 2) == L"\\\\")
    {
        error = "TEXT_GUI_WINLOGO_ERROR_MY_DOCS_FOLDER_REDIRECTED";
    }
    else
    {
        CWideString probePath = myDocumentsPath +
            L"This directory has restricted permissions, and Fable cannot run properly";
        void* file = g_FableCreateFileW_0143FE2C(
            probePath, 0x10000000, 3, 0, 2, 0, 0);
        if (file == reinterpret_cast<void*>(-1))
        {
            error = "TEXT_GUI_WINLOGO_ERROR_MY_DOCS_FOLDER_RESTRICTED";
        }
        else
        {
            g_FableCloseHandle_0143FE30(file);
            FILE* stream = _wfopen(probePath, L"w+");
            if (stream == 0)
            {
                error = "TEXT_GUI_WINLOGO_ERROR_MY_DOCS_FOLDER_RESTRICTED";
            }
            else
            {
                fclose(stream);
                if (!g_FableDeleteFileW_0143FE34(probePath))
                    error = "TEXT_GUI_WINLOGO_ERROR_MY_DOCS_FOLDER_RESTRICTED";
            }
        }
    }

    if (error != "")
        return error.GetCRC();
    return 0;
}
