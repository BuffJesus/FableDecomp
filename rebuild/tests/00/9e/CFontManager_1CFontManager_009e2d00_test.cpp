#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_system_manager.h"
#include "fable_font_data_banks.h"
#include "fable_font_vertex_buffer.h"

static char g_events[16];
static unsigned g_eventCount;
static CFontDataBank* g_expectedBank;
static bool g_closeReceiverValid;
static void Record(char event) { g_events[g_eventCount++] = event; }

void __fastcall RecordClose(CFontDataBank* bank, int)
{
    g_closeReceiverValid = bank == g_expectedBank;
    Record('C');
}
void __fastcall ReleaseStatic(void*) { Record('R'); }
void __fastcall ReleaseStreaming(void*) { Record('T'); }
void __fastcall ReleaseSymbols(void*) { Record('Y'); }
void __fastcall ReleaseStreamingSymbols(void*) { Record('S'); }

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CVertexBufferWin32::~CVertexBufferWin32() { Record('V'); }
bool CVertexBufferWin32::Initialise(
    EVertexType, long, long, unsigned long, EVertexBufferPool)
{
    return true;
}

struct CFontManagerTestAccess
{
    static void Setup(CFontManager* manager, CFontDataBank* bank)
    {
        manager->PFontDataBank.Data = bank;
        manager->PFontDataBank.Info =
            new CCPPointerInfo(&ReleaseStatic, bank);
        manager->PStreamingFontDataBank.Data =
            reinterpret_cast<CStreamingFontDataBank*>(1);
        manager->PStreamingFontDataBank.Info =
            new CCPPointerInfo(&ReleaseStreaming, 0);
        manager->PSymbols.Data = reinterpret_cast<CSymbolMap*>(1);
        manager->PSymbols.Info =
            new CCPPointerInfo(&ReleaseSymbols, 0);
        manager->PStreamingSymbols.Data = reinterpret_cast<CSymbolMap*>(2);
        manager->PStreamingSymbols.Info =
            new CCPPointerInfo(&ReleaseStreamingSymbols, 0);
        manager->VertexBuffers._M_start =
            static_cast<CVertexBufferWin32**>(
                malloc(2 * sizeof(CVertexBufferWin32*)));
        manager->VertexBuffers._M_start[0] = new CVertexBufferWin32;
        manager->VertexBuffers._M_start[1] = new CVertexBufferWin32;
        manager->VertexBuffers._M_finish =
            manager->VertexBuffers._M_start + 2;
        manager->VertexBuffers._M_end_of_storage._M_data =
            manager->VertexBuffers._M_finish;
    }
};

int main()
{
    unsigned char managerStorage[sizeof(CFontManager)];
    unsigned char bankStorage[sizeof(CFontDataBank)];
    void* bankVtable[6] = {0, 0, 0, 0, 0,
                          reinterpret_cast<void*>(&RecordClose)};
    memset(managerStorage, 0, sizeof(managerStorage));
    memset(bankStorage, 0, sizeof(bankStorage));
    CFontManager* manager =
        reinterpret_cast<CFontManager*>(managerStorage);
    g_expectedBank = reinterpret_cast<CFontDataBank*>(bankStorage);
    *reinterpret_cast<void***>(bankStorage) = bankVtable;
    CFontManagerTestAccess::Setup(manager, g_expectedBank);
    manager->CFontManager::~CFontManager();
    g_events[g_eventCount] = 0;
    if (!g_closeReceiverValid || strcmp(g_events, "CRVVSYT") != 0)
        return 1;
    puts("FONT_MANAGER_DTOR_009e2d00_PASS");
    return 0;
}

// Construction/destruction fixture only. Dispatch is checked by the independent native gate.
unsigned char* CVertexBufferWin32::DoLock(unsigned long,unsigned long,unsigned long){abort();return 0;}
void CVertexBufferWin32::DoUnlock(){abort();}
bool CVertexBufferWin32::DoIsBusy() const{abort();return false;}
unsigned long CVertexBufferWin32::DoSizeof() const{abort();return 0;}
