#include "fable_ime.h"

void NUISystem::CIMEFont::SetHeight(unsigned int height)
{
    Height = height;
}

void NUISystem::CIMEFont::SetPosition(int x, int y)
{
    Position.X = static_cast<float>(x);
    Position.Y = static_cast<float>(y);
}

