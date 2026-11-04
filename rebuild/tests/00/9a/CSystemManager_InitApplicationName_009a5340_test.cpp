#include "fable_system_manager_state.h"
#include "test_support/import_redirect.h"
#include <wchar.h>
#include <stdio.h>
#include <string.h>
struct CSystemManagerTestAccess {static void Run(CSystemManager* p) {p->InitApplicationName();}};
static const wchar_t* command;
static const wchar_t* expectedName;
static const CWideString* wide;
static CSystemManager* manager;
static int errors,commands,wideAlive,charAlive,conversions,assignments;
static wchar_t* FABLE_STDCALL CommandLine() {++commands;return const_cast<wchar_t*>(command);}
extern "C" void* NameCommandLineImport=reinterpret_cast<void*>(&CommandLine);
#pragma comment(linker,"/alternatename:__imp__GetCommandLineW@0=_NameCommandLineImport")
CWideString::CWideString(const wchar_t* text) {
    if(wideAlive || !expectedName || wcscmp(text,expectedName)) ++errors;
    wide=this;++wideAlive;storage_=0;
}
CWideString::~CWideString() {if(this!=wide || wideAlive!=1 || charAlive || assignments!=1) ++errors;--wideAlive;}
CCharString::CCharString() {storage_=0;++charAlive;}
CCharString::CCharString(const CCharString& other) {storage_=other.storage_;++charAlive;}
CCharString::~CCharString() {if(!wideAlive || charAlive<=0) ++errors;--charAlive;}
CCharString CWideString::ToCharString() const {
    if(this!=wide || wideAlive!=1) ++errors;
    ++conversions;return CCharString();
}
CCharString& CCharString::operator=(const CCharString&) {
    if(this!=&manager->ApplicationName || wideAlive!=1 || charAlive<=0) ++errors;
    ++assignments;storage_=reinterpret_cast<CCharStringData*>(0x12345678);return *this;
}
static void Run(const wchar_t* input,const wchar_t* filename) {
    command=input;expectedName=filename;commands=wideAlive=charAlive=conversions=assignments=0;wide=0;
    unsigned long storage[0x180/4];memset(storage,0xa5,sizeof(storage));manager=reinterpret_cast<CSystemManager*>(storage);
    unsigned char expected[sizeof(storage)];memcpy(expected,storage,sizeof(storage));
    if(filename) {unsigned long value=0x12345678;memcpy(expected+0x168,&value,4);}
    CSystemManagerTestAccess::Run(manager);
    if(commands!=1 || wideAlive || charAlive || conversions!=int(filename!=0) || assignments!=int(filename!=0) || memcmp(storage,expected,sizeof(storage))) ++errors;
}
int main() {
    FableTest::ImportRedirect commandImport("GetCommandLineW",reinterpret_cast<void*>(&CommandLine));
    const wchar_t* inputs[]={L"",L"fable.exe",L"fable.exe args",L"C:\\Games\\Fable.exe -x",L"\"C:\\Games With Spaces\\Fable.exe\" -x",
        L" \"C:\\Fable.exe\"",L"C:/Games/Fable.exe",L"\\\\server\\share\\Fable.exe",L"C:\\Name WithSpace\\file.exe",L"\"C:\\Name WithSpace\\file.exe\"",
        L"C:\\",L"\"C:\\\"",L"\"C:\\file.exe",L"C:\\foo\"bar.exe ignored",L"C:\\foo.exe\tother",L"\"C:\\f.exe\"ignored",L"\"\"",L"\"",
        L"C:\\\x4e2d\x6587.exe",L"C:\\path\\..\\file.exe",L"C:\\path\\..\\",L"C:\\.exe",L"C:\\a\\b\\c"};
    const wchar_t* names[]={0,0,0,L"Fable.exe",L"Fable.exe",0,0,L"Fable.exe",L"Name",L"file.exe",L"",L"",L"file.exe",L"foo\"bar.exe",L"foo.exe\tother",L"f.exe",0,0,L"\x4e2d\x6587.exe",L"file.exe",L"",L".exe",L"c"};
    unsigned scenarios=0;
    for(unsigned i=0;i<sizeof(inputs)/sizeof(inputs[0]);++i) {Run(inputs[i],names[i]);++scenarios;if(errors){printf("NAME_FAIL case=%u errors=%d\n",i,errors);return 1;}}
    // Longest terminated command copied safely by retail's fixed-size copy.
    wchar_t longName[258],longCommand[261];
    for(i=0;i<257;++i) longName[i]=L'x';longName[257]=0;
    wcscpy(longCommand,L"C:\\");wcscat(longCommand,longName);Run(longCommand,longName);++scenarios;
    if(errors){printf("NAME_FAIL long errors=%d\n",errors);return 1;}
    printf("APPLICATION_NAME_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
