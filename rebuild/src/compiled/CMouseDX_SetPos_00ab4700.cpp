#include "candidates/mouse_dx_accessors_compiled.h"

void CMouseDX::SetPos(const C2DVector* position)
{
    PositionX = position->X;
    CachedPositionX = position->X;
    PositionY = position->Y;
    CachedPositionY = position->Y;

    if (DisableCursorWarp == 0)
    {
        CMousePositionContext* context = reinterpret_cast<CMousePositionContext*>(
            *reinterpret_cast<const fable_u32*>(&position->Y));
        CMousePrimitiveOwner* owner = context->ResolvePrimitiveOwner();
        CMousePoint clientOrigin;
        CMouseRect clientRect;
        clientOrigin.X = 0;
        clientOrigin.Y = 0;
        FableClientToScreen(owner->Window, &clientOrigin);
        FableGetClientRect(owner->Window, &clientRect);
        FableOffsetRect(&clientRect, clientOrigin.X, clientOrigin.Y);
        int x = static_cast<int>(static_cast<float>(clientRect.Left) + PositionX);
        int y = static_cast<int>(static_cast<float>(clientRect.Top) + PositionY);
        FableSetCursorPos(x, y);
    }
}
