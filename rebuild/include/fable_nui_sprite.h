#pragma once

namespace NUISystem
{
    // Call-only interface to the UI sprite, not the unrelated global CSprite.
    // No object construction or storage layout is modeled here. Ego_r.pdb
    // names this nonvirtual method; retail CIME::Render calls it at 0041A980.
    class CSprite
    {
    public:
        void DrawWithRenderManager();
    };
}
