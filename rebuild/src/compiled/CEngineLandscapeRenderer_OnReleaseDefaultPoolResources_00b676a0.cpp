#include "candidates/landscape_renderer_release_compiled.h"

void CEngineLandscapeRenderer::OnReleaseDefaultPoolResources()
{
    int index = 0;
    CEngineLandscapeMapList* list = DAT_01436e8c;
    if (0 < static_cast<int>(
        (reinterpret_cast<int>(list->End) - reinterpret_cast<int>(list->Begin)) & 0xFFFFFFFCU))
    {
        do
        {
            CEngineLandscapeMapEntry* entry = *(reinterpret_cast<CEngineLandscapeMapEntry**>(
                reinterpret_cast<int>(list->Begin) + index * 4));
            if (entry != 0)
            {
                CEngineLandscapeMap* map = entry->LandscapeMap;
                if (map != 0)
                {
                    map->ReleaseDefaultPoolSurfaces();
                    list = DAT_01436e8c;
                }
            }
            ++index;
        } while (index <
            (reinterpret_cast<int>(list->End) - reinterpret_cast<int>(list->Begin)) >> 2);
    }
}
