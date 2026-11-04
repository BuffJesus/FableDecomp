#include "fable_resource_directories.h"
#include "fable_shell_paths.h"
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetMeshBankDir() {
    return L"Data\\Meshes\\";
}
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetEngineCacheDir() {
    return L"Data\\EngineCache\\";
}
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetWriteableEngineCacheDir() {
    wchar_t path[260];
    g_FableGetFolderPathW_0041A430(0,26,0,0,path);
    return CWideString(path)+L"\\Microsoft\\Fable\\";
}
