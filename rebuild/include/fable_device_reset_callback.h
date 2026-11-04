#pragma once

// Retail vtable 0x0122DE50 has exactly two pure slots. Colour-depth changes
// are a separate CGameComponent virtual, not part of this interface.
class CDeviceResetCallback
{
public:
    virtual void OnPreDeviceReset() = 0;
    virtual bool OnPostDeviceReset() = 0;
};
