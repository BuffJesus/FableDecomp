#include "fable_ime.h"
#include "fable_string.h"

void NUISystem::CIMEFont::GetFontNameAndScale(CCharString& name, float& scale) const
{
    const float height = static_cast<float>(Height);
    name = "ENG_ARIAL_";
    if (Height < 16) { name += "12"; scale = height * (1.0f / 12.0f); }
    else if (Height < 18) { name += "16"; scale = height * (1.0f / 16.0f); }
    else if (Height < 24) { name += "18"; scale = height * (1.0f / 18.0f); }
    else if (Height < 28) { name += "24"; scale = height * (1.0f / 24.0f); }
    else { name += "28"; scale = height * (1.0f / 28.0f); }
}
