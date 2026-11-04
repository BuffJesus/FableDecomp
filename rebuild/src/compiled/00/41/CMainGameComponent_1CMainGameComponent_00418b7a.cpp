// CMainGameComponent::~CMainGameComponent @ 0x00418B7A
// Same size-optimised TU as the ctor 0x00418DCA. Everything after the
// Shutdown guard is compiler-generated member destruction in reverse
// declaration order (the scoped_ptr members delete through the virtual
// destructors), then the inline ~CGameComponent.
#pragma optimize("s", on)

#include "fable_game.h"

// Polymorphic views of the scoped_ptr pointees (retail deletes them through
// vtable slot 0).
#include "fable_display_engine.h"
#include "fable_player_manager.h"
#include "fable_world.h"

CMainGameComponent::~CMainGameComponent()
{
    if (!Quit)
        Shutdown();
}