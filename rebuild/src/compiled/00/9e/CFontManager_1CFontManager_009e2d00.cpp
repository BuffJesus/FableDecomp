#include "fable_system_manager.h"
#include "fable_font_data_banks.h"
#include "fable_symbol_map.h"
#include "fable_font_vertex_buffer.h"

CFontManager::~CFontManager()
{
    if (PFontDataBank.Data != 0)
    {
        PFontDataBank.Data->Close();
        if (PFontDataBank.Info != 0)
        {
            --PFontDataBank.Info->RefCount;
            if (PFontDataBank.Info->RefCount == 0)
            {
                PFontDataBank.Info->DeleteFunc(PFontDataBank.Info->Data);
                delete PFontDataBank.Info;
            }
        }
        PFontDataBank.Info = 0;
        PFontDataBank.Data = 0;
    }
    for (unsigned long index = 0; index < VertexBuffers.size(); ++index)
        delete VertexBuffers[index];
}
