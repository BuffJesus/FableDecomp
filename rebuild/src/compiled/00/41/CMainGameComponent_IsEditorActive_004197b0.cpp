#include "fable_game.h"

// Release builds have no editor: vtable slot 9 of CMainGameComponent.
bool CMainGameComponent::IsEditorActive() const
{
    return false;
}
