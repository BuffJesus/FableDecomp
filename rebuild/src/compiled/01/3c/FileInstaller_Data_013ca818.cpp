#include "fable_file_installer.h"

// Shipping .data zero-initialized counted singleton, verified at 013CA818.
CCountedFileInstaller g_CFileInstallerSingleton_013CA818 = {0, 0};
extern const char g_FileInstallerCompressedExtension_0129B448[] = ".biz";
extern const char g_FileInstallerPlainExtension_0129B440[] = ".big";

// Constructor calls 00BFEA0E, the retail CRT malloc import thunk. Resolve the
// descriptive reconstruction name directly to the linked CRT allocator.
#pragma comment(linker, "/alternatename:?FableInstallerContainerAllocate@@YAPAXK@Z=_malloc")
// 00BFEA1A / 00BFE9BC import the VC7.1 scalar new/delete operators.
#pragma comment(linker, "/alternatename:?FableFileInstallerAllocate@@YAPAXI@Z=??2@YAPAXI@Z")
#pragma comment(linker, "/alternatename:?FableOperatorNew@@YAPAXI@Z=??2@YAPAXI@Z")
#pragma comment(linker, "/alternatename:?FableOperatorDelete@@YAXPAX@Z=??3@YAXPAX@Z")
