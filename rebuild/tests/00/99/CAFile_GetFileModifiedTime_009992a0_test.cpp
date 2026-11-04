#include "fable_date_and_time.h"
#include "fable_filesystem.h"
#include "fable_file_information.h"
#include <stdio.h>
#include <string.h>
#include <wchar.h>
extern "C" __declspec(dllimport) int __stdcall VirtualProtect(void*, unsigned long, unsigned long, unsigned long*);
extern "C" char __ImageBase;
    void** FindImportSlot(const char* name)
    {
        char* base = &__ImageBase;
        char* nt = base + *reinterpret_cast<fable_u32*>(base + 0x3C);
        const fable_u32 rva = *reinterpret_cast<fable_u32*>(nt + 0x80);
        if (rva == 0)
            return 0;
        for (fable_u32* entry = reinterpret_cast<fable_u32*>(base + rva);
             entry[3] != 0;
             entry += 5)
        {
            fable_u32* names = reinterpret_cast<fable_u32*>(
                base + (entry[0] ? entry[0] : entry[4]));
            void** slots = reinterpret_cast<void**>(base + entry[4]);
            for (int i = 0; names[i] != 0; ++i)
            {
                if ((names[i] & 0x80000000u) == 0 &&
                    strcmp(base + names[i] + 2, name) == 0)
                {
                    return &slots[i];
                }
            }
        }
        return 0;
    }

    bool SwapImport(const char* name, void* replacement, void** previous)
    {
        void** slot = FindImportSlot(name);
        unsigned long oldProtect = 0;
        if (slot == 0 ||
            !VirtualProtect(slot, sizeof(void*), 0x04, &oldProtect))
        {
            return false;
        }
        if (previous != 0)
            *previous = *slot;
        *slot = replacement;
        VirtualProtect(slot, sizeof(void*), oldProtect, &oldProtect);
        return true;
    }


namespace
{
    const unsigned int kTextCharacters = 300;
    wchar_t g_tempDirectory[kTextCharacters];
    int g_liveStrings;
    unsigned int g_currentPathCalls;
    unsigned int g_expandCalls;
}

// Minimal owning CWideString: storage_ points at a heap copy of the text.
static CWideStringData* NewData(const wchar_t* text)
{
    // Raw block: CWideStringData has no default ctor.
    CWideStringData* data = static_cast<CWideStringData*>(operator new(sizeof(CWideStringData)));
    data->text = new wchar_t[wcslen(text) + 1];
    wcscpy(data->text, text);
    data->owners = 1;
    return data;
}

CWideString::CWideString() : storage_(0) { ++g_liveStrings; }
CWideString::CWideString(const wchar_t* text) : storage_(NewData(text))
{
    ++g_liveStrings;
}
CWideString::CWideString(const CWideString& other)
    : storage_(other.storage_ ? NewData(other.storage_->text) : 0)
{
    ++g_liveStrings;
}
CWideString::~CWideString()
{
    --g_liveStrings;
    if (storage_)
    {
        wchar_t* text = storage_->text;  // (a property in delete[] trips VC7.1)
        delete[] text;
        delete storage_;
    }
}
CWideString& CWideString::operator=(const CWideString& other)
{
    CWideStringData* copy = other.storage_ ? NewData(other.storage_->text) : 0;
    if (storage_)
    {
        wchar_t* text = storage_->text;  // (a property in delete[] trips VC7.1)
        delete[] text;
        delete storage_;
    }
    storage_ = copy;
    return *this;
}
CWideString::operator const wchar_t*() const
{
    return storage_ ? storage_->text : L"";
}

CWideString FABLE_FASTCALL CAFile::GetCurrentPath()
{
    ++g_currentPathCalls;
    return CWideString(g_tempDirectory);
}

// Relative names resolve against the context directory.
CWideString FABLE_FASTCALL CAFile::ExpandPath(
    const CWideString& pathname,
    const CWideString& context)
{
    ++g_expandCalls;
    wchar_t text[kTextCharacters];
    wcscpy(text, context);
    wcscat(text, pathname);
    return CWideString(text);
}


static unsigned queryCalls,constructorCalls;static int resultCode;static bool valid;
struct Attributes {unsigned long Flags;FILETIME Creation,Access,Write;unsigned long High,Low;};
static int __stdcall fakeAttributes(const wchar_t* path,int mode,void* output){
 ++queryCalls;if(mode!=0 || wcscmp(path,L"D:\\fixture\\asset.bin"))valid=false;
 Attributes* a=static_cast<Attributes*>(output);memset(a,0xA5,sizeof(*a));
 a->Write.dwLowDateTime=123;a->Write.dwHighDateTime=456;return resultCode;
}
CDateAndTime::CDateAndTime(const FILETIME& time){
 ++constructorCalls;if(time.dwLowDateTime!=123 || time.dwHighDateTime!=456)valid=false;
 Year=2004;Month=9;Day=14;Hour=12;Minute=34;Second=56;Millisecond=789;
}
static int Run(){
 wcscpy(g_tempDirectory,L"D:\\fixture\\");valid=true;
 for(unsigned n=0;n<2;++n){resultCode=n?0:1;CWideString name(L"asset.bin");CDateAndTime result=CAFile::GetFileModifiedTime(name);
  if(!valid || result.Year!=2004 || result.Millisecond!=789 || g_liveStrings!=1 || queryCalls!=n+1 || constructorCalls!=n+1 || g_currentPathCalls!=n+1 || g_expandCalls!=n+1)return 1;
 }
 return g_liveStrings?2:0;
}
int main(){void* previous=0;if(!SwapImport("GetFileAttributesExW",(void*)&fakeAttributes,&previous))return 90;
 int result=Run();if(!SwapImport("GetFileAttributesExW",previous,0))return 91;
 if(!result)puts("FILE_MODIFIED_TIME_009992a0_PASS");return result;
}
