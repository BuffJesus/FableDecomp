// Native x86 callback/APC probe. Original bytes are supplied privately at runtime.
#include "fable_threaded_file_object.h"
#include "fable_file_installer.h"
#include "NativeThreadedReadAPC_win32.h"
#include <stdio.h>
#include <string.h>

typedef void (__stdcall* Completion)(DWORD,DWORD,OVERLAPPED*);
typedef void (__fastcall* HardError)(const char*);
extern "C" void __stdcall FixtureReadCompletion(DWORD,DWORD,OVERLAPPED*);
extern "C" bool FixtureDirty;
extern "C" HardError FixtureHardError;
extern "C" CThreadedFileManager::ErrorCallback FixtureError;
#pragma comment(linker,"/alternatename:_FixtureReadCompletion@12=?OnFileReadCompletion@CThreadedFileManager@@CGXKKPAU_OVERLAPPED@@@Z")
#pragma comment(linker,"/alternatename:_FixtureDirty=?m_bDirtyDiscError@CThreadedFileManager@@0_NA")
#pragma comment(linker,"/alternatename:_FixtureHardError=?m_harddiskErrorCallback@CThreadedFileManager@@0P6IXPBD@ZA")
#pragma comment(linker,"/alternatename:_FixtureError=?m_errorCallback@CThreadedFileManager@@0P6IXXZA")

CCountedFileInstaller g_CFileInstallerSingleton_013CA818 = {0,0};
static void Fail(const char* expression,int line)
{
    fprintf(stderr,"NATIVE_APC_FAILURE line=%d expr=%s winerr=%lu\n",line,expression,GetLastError());
    fflush(stderr);
    ExitProcess(80);
}
#define CHECK(expr) do { if (!(expr)) Fail(#expr,__LINE__); } while (0)

struct PolicyAccess : CThreadedFileManager {
    using CThreadedFileManager::GetHardDiskErrorCallback;
    using CThreadedFileManager::CallHardDiskErrorCallback;
};
static unsigned g_policyCalls;
static const char* g_policyText;
static void __fastcall PolicyError() {}
static void __fastcall PolicyHard(const char* text) { CHECK(text==g_policyText);++g_policyCalls; }
static void CheckPolicy()
{
    CHECK(!FixtureError && !FixtureHardError && !FixtureDirty);
    CThreadedFileManager::SetErrorCallback(PolicyError);CHECK(FixtureError==PolicyError);
    CThreadedFileManager::SetHardDiskErrorCallback(PolicyHard);CHECK(PolicyAccess::GetHardDiskErrorCallback()==PolicyHard);
    g_policyText="native policy";g_policyCalls=0;
    CThreadedFileManager::DoHardDiskErrorCallback(g_policyText);
    PolicyAccess::CallHardDiskErrorCallback(g_policyText);CHECK(g_policyCalls==2);
    CThreadedFileManager::SetHardDiskErrorCallback(0);
    CThreadedFileManager::DoHardDiskErrorCallback(0);CHECK(g_policyCalls==2);
    CThreadedFileManager::SetDirtyDiscError(true);CHECK(FixtureDirty);
    CThreadedFileManager::SetDirtyDiscError(false);CHECK(!FixtureDirty);
    CThreadedFileManager::SetErrorCallback(0);CHECK(!FixtureError);
}

// The installer is absent in this probe; its real non-null path has another gate.
CFileInstaller& FABLE_FASTCALL CFileInstallerSingleton::Get()
{
    Fail("Unexpected installer construction/lookup",__LINE__);
    return *reinterpret_cast<CFileInstaller*>(0);
}

static Completion g_engine;
static volatile LONG g_completed;
static DWORD g_error,g_transferred,g_hardCalls;
static bool g_redirect;
static CThreadedFileManager::CFileObject* g_request;
static CThreadedFileManager* g_alternate;
static const char g_empty[] = "";
typedef BOOL (WINAPI* SetEventPointer)(HANDLE);
static SetEventPointer g_setEvent = SetEvent;

static void __fastcall HardErrorCallback(const char* text)
{
    CHECK(text && !text[0]);
    ++g_hardCalls;
    if (g_redirect) g_request->PManager = g_alternate;
}

static void CALLBACK Dispatch(DWORD error,DWORD transferred,OVERLAPPED* overlapped)
{
    CHECK(g_completed == 0);
    g_error=error;
    g_transferred=transferred;
    g_engine(error,transferred,overlapped);
    g_completed=1; // APC and alertable wait execute on this same thread.
}

static DWORD GetWord(const unsigned char* bytes,unsigned offset)
{
    DWORD value;
    memcpy(&value,bytes+offset,4);
    return value;
}
static void PutWord(unsigned char* bytes,unsigned offset,DWORD value)
{
    memcpy(bytes+offset,&value,4);
}

static Completion LoadOriginal(const char* path)
{
    unsigned char original[102];
    FILE* input=fopen(path,"rb");
    CHECK(input);
    CHECK(fread(original,1,sizeof(original),input)==sizeof(original));
    CHECK(fgetc(input)==EOF);
    CHECK(fclose(input)==0);
    CHECK(GetWord(original,32)==0x13bc868);
    CHECK(GetWord(original,39)==0x13bc864);
    CHECK(GetWord(original,48)==0x122d70e);
    CHECK(GetWord(original,73)==0x143fde0);
    CHECK(GetWord(original,78)==0x13ca818);
    CHECK(DWORD(0x98e320+88+4+LONG(GetWord(original,88)))==0x404440);
    CHECK(DWORD(0x98e320+95+4+LONG(GetWord(original,95)))==0x9c3250);
    CHECK(original[99]==0xc2 && original[100]==12 && original[101]==0);
    unsigned char* code=static_cast<unsigned char*>(VirtualAlloc(0,4096,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));
    CHECK(code);
    memcpy(code,original,sizeof(original));
    PutWord(code,32,reinterpret_cast<DWORD>(&FixtureDirty));
    PutWord(code,39,reinterpret_cast<DWORD>(&FixtureHardError));
    PutWord(code,48,reinterpret_cast<DWORD>(g_empty));
    PutWord(code,73,reinterpret_cast<DWORD>(&g_setEvent));
    PutWord(code,78,reinterpret_cast<DWORD>(&g_CFileInstallerSingleton_013CA818));
    PutWord(code,88,reinterpret_cast<DWORD>(&CFileInstallerSingleton::Get)-(reinterpret_cast<DWORD>(code)+92));
    typedef void (CFileInstaller::*UnpauseMember)();
    UnpauseMember member=&CFileInstaller::Unpause;
    CHECK(sizeof(member)==4);
    DWORD unpause;
    memcpy(&unpause,&member,4);
    PutWord(code,95,unpause-(reinterpret_cast<DWORD>(code)+99));
    DWORD old;
    CHECK(VirtualProtect(code,4096,PAGE_EXECUTE_READ,&old));
    CHECK(FlushInstructionCache(GetCurrentProcess(),code,102));
    return reinterpret_cast<Completion>(code);
}

struct Result
{
    DWORD accepted,submitError,callbackError,transferred,primary,alternate,dirty,hardCalls,alertable,dataHash;
};
struct ReadCase
{
    DWORD offset,length,device;
    bool hardCallback,redirect;
};

static unsigned char Payload(DWORD index)
{
    return static_cast<unsigned char>(index*37+23);
}

static Result Run(Completion engine,HANDLE file,const ReadCase& spec,bool direct,DWORD directError)
{
    CHECK(sizeof(void*)==4 && sizeof(OVERLAPPED)==20);
    CHECK(sizeof(CThreadedFileManager::CFileObject)==0x40);
    __declspec(align(8)) unsigned long manager[0x178/4],alternate[0x178/4];
    unsigned long fileStorage[0x1c/4];
    memset(manager,0x5a,sizeof(manager));memset(alternate,0x6b,sizeof(alternate));memset(fileStorage,0x3c,sizeof(fileStorage));
    HANDLE primary=CreateEventA(0,1,0,0),secondary=CreateEventA(0,1,0,0);
    CHECK(primary && secondary);
    manager[0x164/4]=reinterpret_cast<DWORD>(primary);
    alternate[0x164/4]=reinterpret_cast<DWORD>(secondary);
    fileStorage[1]=spec.device;
    unsigned char managerBefore[sizeof(manager)],alternateBefore[sizeof(alternate)],fileBefore[sizeof(fileStorage)];
    memcpy(managerBefore,manager,sizeof(manager));memcpy(alternateBefore,alternate,sizeof(alternate));memcpy(fileBefore,fileStorage,sizeof(fileStorage));
    CThreadedFileManager::CFileObject request;
    memset(&request,0xa5,sizeof(request));
    request.PManager=reinterpret_cast<CThreadedFileManager*>(manager);
    request.PFile=reinterpret_cast<CThreadedFile*>(fileStorage);
    request.LoadingStatus=CThreadedFileManager::FILE_LOADING_STATUS_READING;
    request.Desc.Size=spec.length;
    unsigned char objectBefore[sizeof(request)];memcpy(objectBefore,&request,sizeof(request));
    OVERLAPPED overlap;memset(&overlap,0,sizeof(overlap));
    overlap.Offset=spec.offset;
    overlap.hEvent=reinterpret_cast<HANDLE>(&request);
    unsigned char buffer[8192+32];memset(buffer,0xcd,sizeof(buffer));
    CHECK(spec.length<=8192);
    g_engine=engine;g_completed=0;g_error=0xffffffff;g_transferred=0;g_hardCalls=0;
    g_request=&request;g_alternate=reinterpret_cast<CThreadedFileManager*>(alternate);g_redirect=spec.redirect;
    CThreadedFileManager::SetHardDiskErrorCallback(spec.hardCallback ? HardErrorCallback : 0);
    CHECK(PolicyAccess::GetHardDiskErrorCallback()==(spec.hardCallback ? HardErrorCallback : 0));
    unsigned char* dirty=reinterpret_cast<unsigned char*>(&FixtureDirty);*dirty=0x73;
    CHECK(!g_CFileInstallerSingleton_013CA818.object && !g_CFileInstallerSingleton_013CA818.reference);
    Result result;memset(&result,0,sizeof(result));
    if (direct) {
        result.accepted=1;
        Dispatch(directError,0xffffffff,&overlap); // Native ABI control, no IO claim.
    } else {
        SetLastError(ERROR_SUCCESS);
        result.accepted=ReadFileEx(file,buffer+16,spec.length,&overlap,Dispatch)!=0;
        result.submitError=GetLastError();
        CHECK(g_completed==0);
        Sleep(1); // A nonalertable wait must not deliver this APC.
        CHECK(g_completed==0);
        if (result.accepted) {
            DWORD started=GetTickCount();
            while (!g_completed) {
                DWORD value=SleepEx(25,1);
                if (value==WAIT_IO_COMPLETION) result.alertable=1;
                CHECK(GetTickCount()-started<5000);
            }
            CHECK(result.alertable);
        } else {
            SleepEx(0,1);
            CHECK(g_completed==0);
        }
    }
    result.callbackError=g_error;result.transferred=g_transferred;result.hardCalls=g_hardCalls;
    if (result.accepted) {
        CHECK(g_completed==1);
        bool hard=bool(g_error && spec.device!=1 && spec.hardCallback);
        bool redirected=hard && spec.redirect;
        CHECK(g_hardCalls==DWORD(hard));
        unsigned char expected[sizeof(request)];memcpy(expected,objectBefore,sizeof(expected));
        DWORD status=3;memcpy(expected+0x1c,&status,4);
        if (redirected) { DWORD target=reinterpret_cast<DWORD>(alternate);memcpy(expected+0x24,&target,4); }
        CHECK(memcmp(&request,expected,sizeof(expected))==0);
        CHECK(*dirty==(g_error && spec.device==1 ? 1 : 0x73));
        DWORD primaryState=WaitForSingleObject(primary,0),secondaryState=WaitForSingleObject(secondary,0);
        CHECK((primaryState==WAIT_OBJECT_0 || primaryState==WAIT_TIMEOUT) && (secondaryState==WAIT_OBJECT_0 || secondaryState==WAIT_TIMEOUT));
        result.primary=primaryState==WAIT_OBJECT_0;
        result.alternate=secondaryState==WAIT_OBJECT_0;
        CHECK(result.primary==DWORD(!redirected) && result.alternate==DWORD(redirected));
        if (!direct) {
            CHECK(g_transferred<=spec.length);
            for (DWORD i=0;i<g_transferred;++i) CHECK(buffer[16+i]==Payload(spec.offset+i));
            for (DWORD i=0;i<16;++i) CHECK(buffer[i]==0xcd);
            for (DWORD i=16+g_transferred;i<sizeof(buffer);++i) CHECK(buffer[i]==0xcd);
            DWORD hash=2166136261UL;
            for (DWORD i=0;i<g_transferred;++i) hash=(hash^buffer[16+i])*16777619UL;
            result.dataHash=hash;
        }
    } else {
        CHECK(request.LoadingStatus==CThreadedFileManager::FILE_LOADING_STATUS_READING);
        CHECK(memcmp(&request,objectBefore,sizeof(request))==0);
        CHECK(*dirty==0x73 && !g_hardCalls);
        CHECK(WaitForSingleObject(primary,0)==WAIT_TIMEOUT && WaitForSingleObject(secondary,0)==WAIT_TIMEOUT);
    }
    result.dirty=*dirty;
    CHECK(memcmp(manager,managerBefore,sizeof(manager))==0);
    CHECK(memcmp(alternate,alternateBefore,sizeof(alternate))==0);
    CHECK(memcmp(fileStorage,fileBefore,sizeof(fileStorage))==0);
    CHECK(CloseHandle(primary) && CloseHandle(secondary));
    g_request=0;g_alternate=0;g_engine=0;
    request.Overlapped=0; // Borrowed stack context; disarm the real owner destructor.
    return result;
}

static void Compare(const Result& original,const Result& rebuilt)
{
    CHECK(original.accepted==rebuilt.accepted);
    if (!original.accepted) CHECK(original.submitError==rebuilt.submitError);
    CHECK(original.callbackError==rebuilt.callbackError && original.transferred==rebuilt.transferred);
    CHECK(original.primary==rebuilt.primary && original.alternate==rebuilt.alternate);
    CHECK(original.dirty==rebuilt.dirty && original.hardCalls==rebuilt.hardCalls);
    CHECK(original.alertable==rebuilt.alertable && original.dataHash==rebuilt.dataHash);
}

int main(int argc,char** argv)
{
    CHECK(argc==3);
    CHECK(*reinterpret_cast<unsigned char*>(&FixtureDirty)==0 && !FixtureHardError);
    CheckPolicy();
    Completion original=LoadOriginal(argv[1]);
    HANDLE file=CreateFileA(argv[2],GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,0);
    CHECK(file!=INVALID_HANDLE_VALUE);
    const DWORD size=0x9000;
    ReadCase reads[]={{0,1,0,false,false},{0,4096,0,true,false},{17,8192,1,false,false},
        {4095,4096,1,true,false},{size-7,64,0,true,true},{size,64,0,true,true},
        {size+17,64,1,true,true},{0,0,0,true,false},{0,8192,0,true,false}};
    DWORD delivered=0,failed=0,errorCompletions=0;
    for (unsigned i=0;i<sizeof(reads)/sizeof(reads[0]);++i) {
        Result a=Run(original,file,reads[i],false,0);
        Result b=Run(FixtureReadCompletion,file,reads[i],false,0);
        Compare(a,b);
        delivered+=a.accepted;failed+=!a.accepted;errorCompletions+=a.accepted && a.callbackError!=0;
        printf("IO %u accepted=%lu error=%lu transferred=%lu events=%lu/%lu dirty=%lu hard=%lu\n",
            i,a.accepted,a.accepted ? a.callbackError : a.submitError,a.transferred,a.primary,a.alternate,a.dirty,a.hardCalls);
    }
    CHECK(delivered>=6);
    DWORD controls=0;
    DWORD errors[]={0,5,0xffffffff};
    for (unsigned e=0;e<3;++e) for (unsigned device=0;device<2;++device)
        for (unsigned callback=0;callback<2;++callback) for (unsigned redirect=0;redirect<2;++redirect) {
            ReadCase spec={0,64,device,callback!=0,redirect!=0};
            Result a=Run(original,file,spec,true,errors[e]);
            Result b=Run(FixtureReadCompletion,file,spec,true,errors[e]);
            Compare(a,b);++controls;
        }
    CHECK(CloseHandle(file));
    CHECK(VirtualFree(reinterpret_cast<void*>(original),0,MEM_RELEASE));
    printf("NATIVE_READ_APC_PASS io_cases=9 delivered=%lu immediate_failures=%lu error_completions=%lu native_controls=%lu\n",
        delivered,failed,errorCompletions,controls);
    return 0;
}
