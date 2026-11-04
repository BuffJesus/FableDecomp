#include "fable_date_and_time.h"
#include "fable_filesystem.h"
#include "fable_file_information.h"
struct FableFileAttributeData {
 unsigned long Attributes;
 FILETIME CreationTime;
 FILETIME LastAccessTime;
 FILETIME LastWriteTime;
 unsigned long FileSizeHigh;
 unsigned long FileSizeLow;
};
extern "C" __declspec(dllimport) int __stdcall GetFileAttributesExW(const wchar_t*,int,void*);
CDateAndTime __fastcall CAFile::GetFileModifiedTime(const CWideString& filename) {
 CWideString path;
 path=ExpandPath(filename,GetCurrentPath());
 FableFileAttributeData attributes;
 GetFileAttributesExW(path,0,&attributes);
 return CDateAndTime(attributes.LastWriteTime);
}
