#include "fable_system_manager.h"
#include "fable_pending_loaded_glyph.h"
#include "test_support/scratch_buffer_storage.h"
#include "test_support/import_redirect.h"
#include <stdio.h>
#include <string.h>

static unsigned long managerStorage[0x180/4], glyphStorage[512*4], driveStorage[2];
static unsigned long infoStorage[521][3], expectedInfo[521][3];
static unsigned long expectedManager[0x180/4], expectedGlyphs[512*4];
static unsigned events[2200], expectedEvents[2200], used, expectedUsed, errors;
static bool running;
static const unsigned offsets[]={0x58,0x7c,0x84,0x6c,0x74,0x50,0x60,0xf8};
PendingLoadedGlyph* FablePendingGlyphs_0138e074=reinterpret_cast<PendingLoadedGlyph*>(glyphStorage);
extern const unsigned long FablePendingGlyphCount_0129ab30=512;
unsigned long FablePendingGlyphRead_013c5a74, FablePendingGlyphWrite_013c5a78;
fable_u8 CSystemManager::NoInstances;
extern "C" unsigned long* ShutdownDriveStorage=driveStorage;
// The singleton owns two words. Bind directly to fixture storage without a constructor.
extern "C" unsigned long ShutdownDriveWords[2]={};
#pragma comment(linker,"/alternatename:?PClass@?$CTBaseSingletonNoDefaultAlloc@VCDriveManager@@@@0V?$CCountedPointer@VCDriveManager@@@@A=_ShutdownDriveWords")

static CSystemManager* Manager() {return reinterpret_cast<CSystemManager*>(managerStorage);}
static void Event(unsigned value) {if(used<2200)events[used++]=value;else ++errors;}
static void Expect(unsigned value) {expectedEvents[expectedUsed++]=value;}
static void FABLE_FASTCALL Destroy(void* object) {
    unsigned id=(reinterpret_cast<unsigned long>(object)-0x10000000)/16;
    if(id>=521 || infoStorage[id][0]!=0) ++errors;
    Event(1000+id);
}
void __cdecl operator delete(void* pointer) {
    if(!running) return;
    unsigned id=(reinterpret_cast<unsigned long>(pointer)-reinterpret_cast<unsigned long>(infoStorage))/12;
    if(id>=521) {++errors;return;}
    Event(2000+id);
}
bool CInitBaseClass::IsValid() const {
    Event(this==reinterpret_cast<CInitBaseClass*>(managerStorage)?1:2);
    return *reinterpret_cast<const unsigned char*>(reinterpret_cast<const char*>(this)+4)!=0;
}
void CInitBaseClass::Uninitialise() {if(this!=reinterpret_cast<CInitBaseClass*>(managerStorage))++errors;Event(6);reinterpret_cast<unsigned char*>(this)[4]=0;}
void CScratchBuffer::Uninitialise() {if(this!=&GScratchBuffer)++errors;Event(3);reinterpret_cast<unsigned char*>(this)[4]=0;}
void CDisplayManager::RemoveDeviceResetCallback(CDeviceResetCallback* callback) {
    if(this!=Manager()->PDisplayManager.Data || callback!=static_cast<CDeviceResetCallback*>(Manager()->PMeshBankManager.Data))++errors;
    Event(4);
}
static int FABLE_STDCALL DestroyWindowMock(void* window) {if(window!=reinterpret_cast<void*>(0x98765432))++errors;Event(5);return 0;}
static void FABLE_STDCALL CriticalMock(void* section) {if(section!=Manager()->CriticalSection)++errors;Event(7);}
extern "C" __declspec(dllimport) void FABLE_STDCALL DeleteCriticalSection(void* section);
extern "C" void* ShutdownDestroyWindowImport=reinterpret_cast<void*>(&DestroyWindowMock);
extern "C" void* ShutdownCriticalImport=reinterpret_cast<void*>(&CriticalMock);
#pragma comment(linker,"/alternatename:__imp__DestroyWindow@4=_ShutdownDestroyWindowImport")
#pragma comment(linker,"/alternatename:__imp__DeleteCriticalSection@4=_ShutdownCriticalImport")

static void Own(unsigned long* words,unsigned id,unsigned mode) {
    words[0]=mode?0x10000000+id*16:0;
    words[1]=mode?reinterpret_cast<unsigned long>(infoStorage[id]):0;
    infoStorage[id][0]=mode==1?1:mode==2?2:0;
    infoStorage[id][1]=reinterpret_cast<unsigned long>(&Destroy);
    infoStorage[id][2]=0x10000000+id*16;
}
static void ReleaseExpected(unsigned long* words,unsigned id) {
    if(words[1]) {
        --expectedInfo[id][0];
        if(!expectedInfo[id][0]) {Expect(1000+id);Expect(2000+id);}
    }
    words[0]=words[1]=0;
}
int main() {
    FableTest::ImportRedirect criticalRedirect("DeleteCriticalSection",reinterpret_cast<void*>(&CriticalMock));
    if(&DeleteCriticalSection!=&CriticalMock) return 2;
    unsigned scenarios=0;
    const unsigned instanceValues[]={0,1,255};
    for(unsigned valid=0;valid<2;++valid)for(unsigned pattern=0;pattern<4;++pattern)
    for(unsigned window=0;window<4;++window)for(unsigned scratch=0;scratch<2;++scratch)
    for(unsigned instance=0;instance<3;++instance) {
        memset(managerStorage,0xa5,sizeof(managerStorage));memset(glyphStorage,0xa5,sizeof(glyphStorage));
        used=expectedUsed=errors=0;
        reinterpret_cast<unsigned char*>(managerStorage)[4]=valid;
        reinterpret_cast<unsigned char*>(&GScratchBuffer)[4]=scratch;
        Manager()->AttachedToExternalWindow=window/2!=0;Manager()->WindowValid=window%2!=0;
        Manager()->ApplicationWinHandle=reinterpret_cast<void*>(0x98765432);
        for(unsigned i=0;i<8;++i)Own(managerStorage+offsets[i]/4,i,(pattern+i)%4);
        Own(ShutdownDriveWords,8,pattern);
        for(unsigned i=0;i<512;++i)Own(glyphStorage+i*4,9+i,(i==0||i==1||i==255||i==511)?(pattern+i)%4:0);
        memcpy(expectedManager,managerStorage,sizeof(managerStorage));memcpy(expectedGlyphs,glyphStorage,sizeof(glyphStorage));memcpy(expectedInfo,infoStorage,sizeof(infoStorage));
        unsigned long expectedDrive[2];memcpy(expectedDrive,ShutdownDriveWords,8);
        CSystemManager::NoInstances=instanceValues[instance];FablePendingGlyphRead_013c5a74=231;FablePendingGlyphWrite_013c5a78=427;
        Expect(1);
        if(valid) {
            ReleaseExpected(expectedManager+0x58/4,0);ReleaseExpected(expectedManager+0x7c/4,1);ReleaseExpected(expectedDrive,8);
            for(unsigned i=0;i<512;++i){ReleaseExpected(expectedGlyphs+i*4,9+i);expectedGlyphs[i*4+3]=0;}
            ReleaseExpected(expectedManager+0x84/4,2);ReleaseExpected(expectedManager+0x6c/4,3);
            if(expectedManager[0x60/4])Expect(4);
            ReleaseExpected(expectedManager+0x74/4,4);Expect(2);if(scratch)Expect(3);
            ReleaseExpected(expectedManager+0x50/4,5);ReleaseExpected(expectedManager+0x60/4,6);
            if(window==1){Expect(5);reinterpret_cast<unsigned char*>(expectedManager)[0xe8]=0;}
            if(window)expectedManager[0x94/4]=0;
            ReleaseExpected(expectedManager+0xf8/4,7);Expect(7);Expect(6);reinterpret_cast<unsigned char*>(expectedManager)[4]=0;
        }
        running=true;Manager()->Uninitialise();running=false;++scenarios;
        if(errors || used!=expectedUsed || memcmp(events,expectedEvents,used*sizeof(unsigned)) ||
           memcmp(managerStorage,expectedManager,sizeof(managerStorage)) || memcmp(glyphStorage,expectedGlyphs,sizeof(glyphStorage)) ||
           memcmp(infoStorage,expectedInfo,sizeof(infoStorage)) || memcmp(ShutdownDriveWords,expectedDrive,8) ||
           CSystemManager::NoInstances!=static_cast<unsigned char>(instanceValues[instance]-valid) ||
           FablePendingGlyphRead_013c5a74!=(valid?0:231) || FablePendingGlyphWrite_013c5a78!=(valid?0:427) ||
           reinterpret_cast<unsigned char*>(&GScratchBuffer)[4]!=(valid?0:scratch)) {
            printf("SYSTEM_SHUTDOWN_FAIL scenario=%u errors=%u events=%u expected=%u\n",scenarios,errors,used,expectedUsed);return 1;
        }
    }
    printf("SYSTEM_SHUTDOWN_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
