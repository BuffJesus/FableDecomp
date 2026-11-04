#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "fable_filesystem.h"
#include "fable_threaded_file.h"
#include "test_support/base_class_stubs.h"

// The fixture links the candidate object and defines the engine members it
// calls as recorders. CreateFileW/GetFileSize are the real kernel32 imports,
// run against a real temporary file.

extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* object);
extern "C" __declspec(dllimport) unsigned long __stdcall GetTempPathW(
    unsigned long length, wchar_t* buffer);

namespace
{
    wchar_t g_realPath[260];
    const wchar_t g_missingPath[] = L"Z:\\fabletlc\\no\\such\\file.big";
    const wchar_t* g_fullPathText;
    const wchar_t* g_driveText;

    unsigned long g_wideDataRaw_[16][4];
    CWideStringData* const g_wideData = reinterpret_cast<CWideStringData*>(g_wideDataRaw_);
    int g_wideDataUsed;
    CCharStringData g_charData[16];
    char g_charText[16][8];
    int g_charDataUsed;

    const CWideString* g_expectedName;
    const CWideString* g_splitPath;
    bool g_argumentsValid;
    int g_wideDestructs;
    int g_charDestructs;
    int g_assigns;
    const CWideString* g_assignSource;
    const CWideString* g_assignDestination;
    int g_upperCalls;

    CWideStringData* NewWideData(const wchar_t* text)
    {
        CWideStringData* data = &g_wideData[g_wideDataUsed++ & 15];
        data->text = const_cast<wchar_t*>(text);
        return data;
    }

    void Reset(const wchar_t* fullPath, const wchar_t* drive)
    {
        g_fullPathText = fullPath;
        g_driveText = drive;
        g_argumentsValid = true;
        g_wideDestructs = 0;
        g_charDestructs = 0;
        g_assigns = 0;
        g_assignSource = 0;
        g_assignDestination = 0;
        g_upperCalls = 0;
        g_splitPath = 0;
    }
}

CWideString::CWideString()
{
    storage_ = 0;
}

CWideString::CWideString(const wchar_t* text)
{
    storage_ = text ? NewWideData(text) : 0;
}

CWideString::~CWideString()
{
    ++g_wideDestructs;
}

CWideString::operator const wchar_t*() const
{
    return storage_ ? storage_->text : L"";
}

CWideString& CWideString::operator=(const CWideString& other)
{
    ++g_assigns;
    g_assignSource = &other;
    g_assignDestination = this;
    storage_ = other.storage_;
    return *this;
}

CCharString::CCharString(const char* text, long length)
{
    if (text == 0)
    {
        storage_ = 0;
        return;
    }
    const int index = g_charDataUsed++ & 15;
    memcpy(g_charText[index], text, length);
    g_charText[index][length] = 0;
    storage_ = &g_charData[index];
    storage_->text = g_charText[index];
}

CCharString::CCharString(const CCharString& other)
{
    storage_ = other.storage_;
}

CCharString::~CCharString()
{
    ++g_charDestructs;
}

CCharString CWideString::ToCharString() const
{
    if (storage_ == 0)
        return CCharString(static_cast<const char*>(0), 0);
    char narrow[8];
    long length = 0;
    for (const wchar_t* p = storage_->text; *p && length < 7; ++p)
        narrow[length++] = static_cast<char>(*p);
    return CCharString(narrow, length);
}

CCharString CCharString::GetAsUpperCase() const
{
    ++g_upperCalls;
    if (storage_ == 0)
        return CCharString(static_cast<const char*>(0), 0);
    char upper[8];
    long length = 0;
    for (const char* p = storage_->text; *p && length < 7; ++p)
        upper[length++] = (*p >= 'a' && *p <= 'z') ? *p - 32 : *p;
    return CCharString(upper, length);
}

template<> bool CBasicString<char>::operator==(const char* other) const
{
    return strcmp(PData, other) == 0;
}

CWideString FABLE_FASTCALL CAFile::ConvertPathToFullPath(
    const CWideString& path)
{
    g_argumentsValid = g_argumentsValid && &path == g_expectedName;
    return CWideString(g_fullPathText);
}

void FABLE_FASTCALL CAFile::SplitPath(
    const CWideString& path,
    CWideString* drive,
    CWideString* directory,
    CWideString* filename,
    CWideString* extension)
{
    g_splitPath = &path;
    g_argumentsValid =
        g_argumentsValid &&
        wcscmp(path, g_fullPathText) == 0 &&
        drive != 0 && directory == 0 && filename == 0 && extension == 0;
    // Written through the storage the way the real SplitPath fills its outputs.
    *reinterpret_cast<CWideStringData**>(drive) =
        g_driveText ? NewWideData(g_driveText) : 0;
}

namespace
{
    bool CheckOpen(
        const wchar_t* drive,
        bool noCaching,
        fable_u32 expectedDevice,
        unsigned long expectedSortKey)
    {
        CWideString name(L"Data\\Levels\\test.big");
        Reset(g_realPath, drive);
        g_expectedName = &name;

        CThreadedFile* file = new CThreadedFile;
        const bool result = file->Open(name, noCaching);
        const bool passed =
            result &&
            g_argumentsValid &&
            file->deviceId_ == expectedDevice &&
            file->fileHandle_ != reinterpret_cast<void*>(-1) &&
            g_assigns == 1 &&
            g_assignSource == &name &&
            g_assignDestination == &file->filenameStorage_ &&
            wcscmp(file->filenameStorage_, L"Data\\Levels\\test.big") == 0 &&
            file->length_ == 1234 &&
            file->physicalSortKey_ == expectedSortKey &&
            !file->openedForWrite_ &&
            file->openFlag_ &&
            g_upperCalls == 1 &&
            g_charDestructs == 2 &&
            g_wideDestructs == 2;
        if (result)
            CloseHandle(file->fileHandle_);
        return passed;
    }

    bool CheckMissing()
    {
        CWideString name(L"Data\\Levels\\missing.big");
        Reset(g_missingPath, L"z:");
        g_expectedName = &name;

        CThreadedFile* file = new CThreadedFile;
        const bool result = file->Open(name, false);
        return
            !result &&
            g_argumentsValid &&
            file->deviceId_ == 0 &&
            file->fileHandle_ == reinterpret_cast<void*>(-1) &&
            g_assigns == 0 &&
            file->length_ == 0 &&
            file->physicalSortKey_ == 0 &&
            !file->openedForWrite_ &&
            !file->openFlag_ &&
            g_charDestructs == 2 &&
            g_wideDestructs == 2;
    }

    bool MakeRealFile()
    {
        GetTempPathW(200, g_realPath);
        wcscat(g_realPath, L"fabletlc_threaded_open.big");
        FILE* stream = _wfopen(g_realPath, L"wb");
        if (stream == 0)
            return false;
        static char payload[1234];
        fwrite(payload, 1, sizeof(payload), stream);
        fclose(stream);
        return true;
    }
}

// The fixture's CThreadedFile ctor/dtor (the real ones are separate leaves).
CThreadedFile::CThreadedFile()
{
    deviceId_ = 0;
    fileHandle_ = reinterpret_cast<void*>(-1);
    length_ = 0;
    physicalSortKey_ = 0;
    openedForWrite_ = true;
    openFlag_ = false;
}

CThreadedFile::~CThreadedFile()
{
}

CBaseClassNonCopyable::CBaseClassNonCopyable()
{
}

CBaseClassNonCopyable::~CBaseClassNonCopyable()
{
}

int main()
{
    if (!MakeRealFile())
    {
        printf("FABLETLC_THREADED_FILE_OPEN FAIL (temp file)\n");
        return 1;
    }

    const bool passed =
        CheckOpen(L"c:", false, 0, 1) &&
        CheckMissing() &&
        CheckOpen(L"d:", true, 1, 2) &&
        CheckOpen(0, false, 0, 3);
    _wremove(g_realPath);
    if (!passed)
    {
        printf("FABLETLC_THREADED_FILE_OPEN FAIL\n");
        return 1;
    }

    printf("FABLETLC_THREADED_FILE_OPEN PASS\n");
    return 0;
}
