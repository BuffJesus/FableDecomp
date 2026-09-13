#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdio>
#include <vector>
#include <fstream>
#include <cstring>
// Inert mapping only. No PE entry point, TLS callbacks, or game initialization.
// Caller verifies the pinned SHA-256 before execution via the Python driver.
static BYTE reservedNativeImage[0x2000000];
int main(int argc, char** argv) {
    if (argc != 2) return 10;
    std::ifstream file(argv[1], std::ios::binary);
    std::vector<char> bytes((std::istreambuf_iterator<char>(file)), {});
    if (bytes.size() < 0x1000) return 11;
    auto dos = reinterpret_cast<IMAGE_DOS_HEADER*>(bytes.data());
    auto nt = reinterpret_cast<IMAGE_NT_HEADERS32*>(bytes.data()+dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE || nt->OptionalHeader.ImageBase != 0x400000) return 12;
    // Copy only four 64K regions into a bounds-checked reserved BSS array.
    // The probe links at 400000 so its image reservation keeps loader/heaps
    // out of these original retail virtual addresses.
    const DWORD regions[]={0xC50000,0x12A0000,0x12B0000,0x1440000};
    auto sections = IMAGE_FIRST_SECTION(nt);
    for (DWORD base:regions) {
        auto block=reinterpret_cast<BYTE*>(base);
        if (block<reservedNativeImage || block+0x10000>reservedNativeImage+sizeof(reservedNativeImage)) return 13;
        for (unsigned offset=0;offset<0x10000;++offset) {
            DWORD rva=base+offset-0x400000;
            for (unsigned i=0;i<nt->FileHeader.NumberOfSections;++i) {
                const auto& s=sections[i];
                if (rva>=s.VirtualAddress && rva-s.VirtualAddress<s.SizeOfRawData)
                    block[offset]=bytes[s.PointerToRawData+rva-s.VirtualAddress];
            }
        }
    }
    auto ole = LoadLibraryW(L"ole32.dll");
    *reinterpret_cast<FARPROC*>(0x144063C)=GetProcAddress(ole,"CoInitializeEx");
    *reinterpret_cast<FARPROC*>(0x1440640)=GetProcAddress(ole,"CoCreateInstance");
    *reinterpret_cast<FARPROC*>(0x1440648)=GetProcAddress(ole,"CoUninitialize");
    // Only native functions C53260..C533D6 need execution permission.
    DWORD prior=0;
    if (!VirtualProtect(reinterpret_cast<void*>(0xC53000),0x1000,PAGE_EXECUTE_READ,&prior)) return 14;
    HWND window=CreateWindowExW(0,L"STATIC",L"Native retail audio probe",WS_POPUP,
                               0,0,1,1,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);
    struct Mode { DWORD vtable, value, com; } mode={0x12B5BB0,0x80000000,0};
    using Init=bool(__thiscall*)(Mode*,const GUID*,HWND,bool);
    using End=void(__thiscall*)(Mode*);
    bool ok=reinterpret_cast<Init>(0xC53260)(&mode,nullptr,window,false);
    std::printf("{\"native_init_ok\":%s,\"native_mode\":%lu,\"com_present\":%s}\n",
                ok?"true":"false",mode.value,mode.com?"true":"false");
    if(mode.com) reinterpret_cast<End>(0xC53310)(&mode);
    DestroyWindow(window);
    FreeLibrary(ole);
    return ok?0:15;
}
