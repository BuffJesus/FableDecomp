// CMouseDX's small overrides (retail 0x00AB46E0 and 0x00AB5F00..0x00AB5F90; retail emits the one-liners as COMDATs
// beside the vtable): the position, the input/recentring/cursor/filtering switches and the button states.
#include "fable_mouse_dx.h"

// 0x00AB46E0
void CMouseDX::GetPos(C2DVector* position)
{
    *position = Pos;
}

// 0x00AB5F00
const C2DVector& CMouseDX::PeekPos() const
{
    return Pos;
}

// 0x00AB5F10
void CMouseDX::SetInputOn(bool on)
{
    InputEnabled = on;
}

// 0x00AB5F20
void CMouseDX::SetRecentringOn(bool on)
{
    ForceRecentre = on;
}

// 0x00AB5F30
bool CMouseDX::IsRecentringOn() const
{
    return ForceRecentre;
}

// 0x00AB5F40
void CMouseDX::SetDrawCursor(bool draw)
{
    DrawingCursor = draw;
}

// 0x00AB5F50
bool CMouseDX::IsDrawingCursor() const
{
    return DrawingCursor;
}

// 0x00AB5F60
void CMouseDX::SetMouseFiltering(bool on)
{
    Filtering = on;
}

// 0x00AB5F70
bool CMouseDX::IsLMBDown() const
{
    return LMBOn;
}

// 0x00AB5F80
bool CMouseDX::IsMMBDown() const
{
    return MMBOn;
}

// 0x00AB5F90
bool CMouseDX::IsRMBDown() const
{
    return RMBOn;
}
