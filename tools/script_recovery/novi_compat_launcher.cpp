// Local-only x86 launcher. Caller validates pinned files before invoking.
#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <cstdio>
#include <cwchar>
#include <vector>

static DWORD RemoteModule(DWORD pid, const wchar_t* name) {
    HANDLE snap=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,pid);
    if(snap==INVALID_HANDLE_VALUE) return 0;
    MODULEENTRY32W entry={sizeof(entry)};DWORD result=0;
    if(Module32FirstW(snap,&entry)) do {
        if(!_wcsicmp(entry.szModule,name)) { result=(DWORD)entry.modBaseAddr;break; }
    } while(Module32NextW(snap,&entry));
    CloseHandle(snap);return result;
}
static bool Call(HANDLE process,DWORD address,void* arg,DWORD& result) {
    HANDLE thread=CreateRemoteThread(process,nullptr,0,(LPTHREAD_START_ROUTINE)address,arg,0,nullptr);
    if(!thread) return false;
    DWORD wait=WaitForSingleObject(thread,30000);
    bool ok=wait==WAIT_OBJECT_0 && GetExitCodeThread(thread,&result);
    CloseHandle(thread);return ok;
}
static bool Inject(HANDLE process,DWORD pid,const wchar_t* path,DWORD& module) {
    // ntdll is mapped before process initialization, unlike kernel32/KernelBase.
    HMODULE ntdll=GetModuleHandleW(L"ntdll.dll");
    FARPROC ldr=GetProcAddress(ntdll,"LdrLoadDll");
    MEMORY_BASIC_INFORMATION info={};BYTE actual[32];SIZE_T read=0;
    if(!ldr || !VirtualQueryEx(process,ldr,&info,sizeof(info)) || info.Type!=MEM_IMAGE ||
       info.AllocationBase!=ntdll || !ReadProcessMemory(process,ldr,actual,sizeof(actual),&read) ||
       read!=sizeof(actual) || memcmp(actual,(void*)ldr,sizeof(actual))) return false;
    SIZE_T size=(wcslen(path)+1)*sizeof(wchar_t);
    if(size>65534) return false;
    void* memory=VirtualAllocEx(process,nullptr,size+16,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
    if(!memory) return false;
    std::vector<BYTE> data(size+16,0);
    *(WORD*)&data[0]=(WORD)(size-2);*(WORD*)&data[2]=(WORD)size;
    *(DWORD*)&data[4]=(DWORD)memory+16;memcpy(data.data()+16,path,size);
    void* code=VirtualAllocEx(process,nullptr,64,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
    if(!code) {VirtualFreeEx(process,memory,0,MEM_RELEASE);return false;}
    std::vector<BYTE> bytes;
    auto imm=[&](BYTE opcode,DWORD value){bytes.push_back(opcode);for(int i=0;i<4;i++)bytes.push_back((BYTE)(value>>(i*8)));};
    imm(0x68,(DWORD)memory+8);imm(0x68,(DWORD)memory);
    bytes.insert(bytes.end(),{0x6a,0,0x6a,0});imm(0xb8,(DWORD)ldr);
    bytes.insert(bytes.end(),{0xff,0xd0});imm(0xa3,(DWORD)memory+12);
    bytes.insert(bytes.end(),{0x85,0xc0,0x78,8});imm(0xa1,(DWORD)memory+8);
    bytes.insert(bytes.end(),{0xc2,4,0,0x31,0xc0,0xc2,4,0});
    SIZE_T written=0;DWORD old=0;
    bool ok=WriteProcessMemory(process,memory,data.data(),data.size(),&written) && written==data.size() &&
            WriteProcessMemory(process,code,bytes.data(),bytes.size(),&written) && written==bytes.size() &&
            VirtualProtectEx(process,code,64,PAGE_EXECUTE_READ,&old) &&
            FlushInstructionCache(process,code,bytes.size()) && Call(process,(DWORD)code,nullptr,module) && module;
    // On timeout terminate the suspended process before freeing pending arguments.
    if(!ok) { TerminateProcess(process,20);WaitForSingleObject(process,5000); }
    VirtualFreeEx(process,code,0,MEM_RELEASE);
    VirtualFreeEx(process,memory,0,MEM_RELEASE);return ok;
}
int wmain(int argc,wchar_t** argv) {
    if(argc!=5) { fwprintf(stderr,L"usage: loader GAME_EXE GAME_DIRECTORY ORIGINAL_DLL ADDON_DLL\n");return 2; }
    // Resolve export locally without running the sidecar's DllMain.
    HMODULE mapped=LoadLibraryExW(argv[4],nullptr,DONT_RESOLVE_DLL_REFERENCES);
    if(!mapped) return 3;
    FARPROC start=GetProcAddress(mapped,"_NoviCompatibilityStart@4");
    if(!start) {FreeLibrary(mapped);return 4;}
    DWORD startRva=(DWORD)start-(DWORD)mapped;FreeLibrary(mapped);
    STARTUPINFOW si={sizeof(si)};PROCESS_INFORMATION pi={};
    std::wstring command=L"\""+std::wstring(argv[1])+L"\"";
    if(!CreateProcessW(argv[1],&command[0],nullptr,nullptr,FALSE,CREATE_SUSPENDED,nullptr,argv[2],&si,&pi)) return 5;
    DWORD original=0,addon=0,status=0;int code=0;
    if(RemoteModule(pi.dwProcessId,L"FableScriptExtender.dll")) code=6;
    else if(!Inject(pi.hProcess,pi.dwProcessId,argv[3],original)) code=7;
    else if(!Inject(pi.hProcess,pi.dwProcessId,argv[4],addon)) code=8;
    else if(!Call(pi.hProcess,addon+startRva,nullptr,status) || status!=1) code=9;
    else if(ResumeThread(pi.hThread)==(DWORD)-1) code=10;
    if(code) { TerminateProcess(pi.hProcess,code);WaitForSingleObject(pi.hProcess,5000); }
    printf("{\"pid\":%lu,\"original_module\":%lu,\"addon_module\":%lu,\"start_status\":%lu,\"result\":%d}\n",
           pi.dwProcessId,original,addon,status,code);
    CloseHandle(pi.hThread);CloseHandle(pi.hProcess);return code;
}
