#include "fable_file_installer.h"
#include "fable_critical_section_lock.h"
#include "engine/CCompressorZlib.h"

extern "C" __declspec(dllimport) unsigned long __stdcall WaitForSingleObjectEx(void*, unsigned long, int);

void CFileInstaller::DecompressionThreadMain()
{
    unsigned char* bytes=reinterpret_cast<unsigned char*>(this);
    while (!bytes[0x7a]) {
        CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(bytes+4),false);
        unsigned char* active=*reinterpret_cast<unsigned char**>(bytes+0x54);
        bool ready=false;
        if(active) {
            CCompressorZlib& compressor=*reinterpret_cast<CCompressorZlib*>(active+0x38);
            if(compressor.HasStreamInput() || **reinterpret_cast<void***>(active+0x94)!=*reinterpret_cast<void**>(active+0x94))
                if(*reinterpret_cast<long*>(bytes+0x30)>0) ready=true;
        }
        lock.Release();
        if(ready) ProcessActiveFileForDecompression(**reinterpret_cast<CActiveFile**>(bytes+0x54));
        else WaitForSingleObjectEx(*reinterpret_cast<void**>(bytes+0x88),100,1);
    }
    bytes[0x79]=1;
}
