#include "fable_system_manager_state.h"
#include <wchar.h>
extern "C" __declspec(dllimport) wchar_t* FABLE_STDCALL GetCommandLineW();

void CSystemManager::InitApplicationName()
{
    wchar_t command[261];
    wchar_t filename[261] = {0};
    wchar_t delimiter=L' ';
    wcsncpy(command,GetCommandLineW(),261);
    if(command[0]==L'"') {
        delimiter=L'"';
        int length=static_cast<int>(wcslen(command));
        for(int i=0;i<length;++i) command[i]=command[i+1];
    }
    wchar_t* end=command;
    while(*end && *end!=delimiter) ++end;
    *end=0;
    wchar_t* separator=wcsrchr(command,L'\\');
    if(separator) {
        wcscpy(filename,separator+1);
        ApplicationName=CWideString(filename).ToCharString();
    }
}
