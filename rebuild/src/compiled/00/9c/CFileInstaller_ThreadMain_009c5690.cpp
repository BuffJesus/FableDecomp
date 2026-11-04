#include "fable_file_installer.h"
#include "fable_critical_section_lock.h"
extern "C" __declspec(dllimport) unsigned long __stdcall WaitForSingleObjectEx(void*,unsigned long,int);
extern "C" __declspec(dllimport) int __stdcall SetEvent(void*);

void CFileInstaller::ThreadMain()
{
    unsigned char* bytes=reinterpret_cast<unsigned char*>(this);
    unsigned long* words=reinterpret_cast<unsigned long*>(this);
    typedef unsigned long(__stdcall* Wait)(void*,unsigned long,int);
    Wait wait=WaitForSingleObjectEx;
    while(!bytes[0x7a]) {
        CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(bytes+4),false);
        void** head=*reinterpret_cast<void***>(bytes+0x38);
        if(*head!=head) break;
        lock.Release();wait(*reinterpret_cast<void**>(bytes+0x84),~0UL,1);
    }
    bytes[0x7b]=1;
    while(!bytes[0x7a]) {
        CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(bytes+4),false);
        if(words[0x7c/4]) {
            while(*reinterpret_cast<long*>(bytes+0x58)>0 || *reinterpret_cast<long*>(bytes+0x5c)>0) {
                lock.Release();wait(*reinterpret_cast<void**>(bytes+0x84),~0UL,1);lock.Lock();
                CCriticalSectionLock completion(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(bytes+4),false);
                if(words[0x74/4]) {
                    PostFileReadCompletion(*reinterpret_cast<CActiveRead**>(bytes+0x74));words[0x74/4]=0;
                }
            }
            lock.Release();SetEvent(*reinterpret_cast<void**>(bytes+0x8c));
            wait(*reinterpret_cast<void**>(bytes+0x90),~0UL,1);lock.Lock();
        }
        if(bytes[0x60]) {
            if(words[0x54/4] && static_cast<long>(words[0x5c/4]+words[0x58/4])<1)
                ProcessActiveFileForWrite(**reinterpret_cast<CActiveFile**>(bytes+0x54));
        } else ProcessReads();
        ProcessAlways();
        if(bytes[0x60]) {
            if(words[0x34/4]*2<words[0x68/4]) {
                unsigned char* active=*reinterpret_cast<unsigned char**>(bytes+0x54);
                if(!active || (**reinterpret_cast<void***>(active+0x90)==*reinterpret_cast<void**>(active+0x90) &&
                               **reinterpret_cast<void***>(active+0x94)==*reinterpret_cast<void**>(active+0x94))) bytes[0x60]=0;
            }
        } else if(words[0x34/4]*2>=words[0x68/4] ||
                  (!words[0x50/4] && **reinterpret_cast<void***>(bytes+0x3c)!=*reinterpret_cast<void**>(bytes+0x3c))) bytes[0x60]=1;
        lock.Release();wait(*reinterpret_cast<void**>(bytes+0x84),100,1);
    }
    CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(bytes+4),false);
    bytes[0x78]=1;bytes[0x7b]=0;lock.Release();
}
