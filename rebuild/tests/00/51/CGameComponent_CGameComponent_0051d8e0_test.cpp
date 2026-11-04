#include "fable_game.h"
#include <new>
#include <stdio.h>
#include <string.h>
#include "test_support/base_class_stubs.h"

static const void* baseThis;
static int baseCalls;
CBaseClassNonCopyable::CBaseClassNonCopyable() { baseThis = this; ++baseCalls; }
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
// ~CGameComponent is inline and empty (retail 0x00415940); no stand-in.
void CGameComponent::Init() {}
void CGameComponent::SetQuit() {}

class ProbeComponent : public CGameComponent
{
public:
    ProbeComponent(CGame& game) : CGameComponent(game) {}
    bool InitialFlags() const { return !Quit && !Running; }
    CGame* Owner() const { return Game; }
    virtual bool Run(CGameComponent**) { return false; }
    virtual void ChangeTextureColourDepth(long) {}
    virtual void OnPreDeviceReset() {}
    virtual bool OnPostDeviceReset() { return true; }
};

int main()
{
    unsigned long gameStorage[sizeof(CGame) / 4];
    unsigned long storage[sizeof(ProbeComponent) / 4];
    memset(storage, 0xA5, sizeof(storage));
    CGame& game = *reinterpret_cast<CGame*>(gameStorage);
    ProbeComponent* probe = new (storage) ProbeComponent(game);
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(storage);
    const bool ok = baseCalls == 1 && baseThis == storage &&
        probe->InitialFlags() && probe->Owner() == &game &&
        bytes[10] == 0xA5 && bytes[11] == 0xA5 &&
        reinterpret_cast<const unsigned char*>(static_cast<CDeviceResetCallback*>(probe)) == bytes + 4;
    probe->~ProbeComponent();
    puts(ok ? "FABLETLC_GAME_COMPONENT_CTOR PASS" : "FABLETLC_GAME_COMPONENT_CTOR FAIL");
    return ok ? 0 : 1;
}