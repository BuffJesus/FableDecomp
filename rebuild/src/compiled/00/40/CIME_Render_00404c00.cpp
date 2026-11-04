#include "fable_ime.h"
#include "fable_nui_component.h"
#include "fable_nui_manager.h"
#include "fable_nui_sprite.h"

extern unsigned char g_IMEAlphaDefault_013750B0;

void NUISystem::CIME::Render()
{
    if (IsInitialised)
    {
        // CEditBox -> CChangingStateComponent -> CComponent are offset-zero
        // bases in the donor layout and the retail slot-27 call.
        CComponent* edit = reinterpret_cast<CComponent*>(PEditBox);
        g_IMEAlphaDefault_013750B0 = edit ? edit->GetRenderColour().A : 0;
        ImeUi_RenderUI(true, true);
        CComponent* cursor = CManager::GetInstance()->PMouseCursor;
        if (cursor)
            reinterpret_cast<CSprite*>(cursor)->DrawWithRenderManager();
    }
}
