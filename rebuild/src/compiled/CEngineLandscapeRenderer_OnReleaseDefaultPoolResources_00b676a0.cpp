#include "candidates/landscape_renderer_release_compiled.h"

// NOTE: This .cpp fix must be paired with a one-line header change in
// rebuild/include/candidates/landscape_renderer_release_compiled.h:
//   change  `void OnReleaseDefaultPoolResources();`
//   to      `virtual void OnReleaseDefaultPoolResources();`
// Retail mangles as ?...@CEngineLandscapeRenderer@@UAEXXZ (virtual thiscall);
// the current header emits @@QAE (non-virtual). The `virtual` keyword is only
// legal on the in-class declaration, so it cannot live in this out-of-line
// definition -- the header edit is required to flip @@QAE -> @@UAE. With that
// edit, this body assembles to the exact 69-byte retail sequence (all bytes
// match apart from the 3 masked relocation fields).
//
// Body shape: reload the container global (DAT_01436e8c) at the top AND bottom
// of each iteration and index through list->Begin[index]. This stops VC7.1
// from hoisting/strength-reducing Begin into a register, so it keeps the
// container pointer live in edx, loads End(+0x24) before Begin(+0x20), and
// re-reads [edx+0x20] each pass -- matching retail and eliminating the loop
// alignment pad.

void CEngineLandscapeRenderer::OnReleaseDefaultPoolResources()
{
    int index = 0;
    CEngineLandscapeMapList* list = DAT_01436e8c;
    if (((reinterpret_cast<int>(list->End) - reinterpret_cast<int>(list->Begin)) & (int)0xFFFFFFFC) > 0)
    {
        do
        {
            list = DAT_01436e8c;
            CEngineLandscapeMapEntry* entry = list->Begin[index];
            if (entry != 0 && entry->LandscapeMap != 0)
            {
                entry->LandscapeMap->ReleaseDefaultPoolSurfaces();
            }
            ++index;
            list = DAT_01436e8c;
        } while (index <
            ((reinterpret_cast<int>(list->End) - reinterpret_cast<int>(list->Begin)) >> 2));
    }
}
