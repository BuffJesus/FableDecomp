#include <stdio.h>

struct C2DVector {
    float X;
    float Y;
};

enum EInputDeviceType { INPUT_DEVICE_MOUSE = 3 };
enum EInputEventType { INPUT_EVENT_EXTRA_MOUSE_RELEASED_BASE = 32 };

struct CInputEventRetail {
    unsigned char EarlierData[0x18];
    C2DVector MousePosition;
    EInputDeviceType DeviceType;
    long DeviceNumber;
    EInputEventType Type;
    float CurrentTime;
    float StartTime;

    void SetAsExtraMouseButtonReleased(
        const C2DVector&, double, double, long);
};

int main()
{
    CInputEventRetail event = {};
    C2DVector position = { 320.5f, 175.25f };
    event.DeviceNumber = 7;
    event.SetAsExtraMouseButtonReleased(position, 12.75, 13.5, 2);

    if (event.DeviceType != INPUT_DEVICE_MOUSE ||
        event.Type != static_cast<EInputEventType>(34) ||
        event.DeviceNumber != 7)
        return 1;
    if (event.MousePosition.X != 320.5f ||
        event.MousePosition.Y != 175.25f)
        return 2;
    if (event.StartTime != 12.75f || event.CurrentTime != 13.5f)
        return 3;

    printf("EXTRA_MOUSE_RELEASED PASS\n");
    return 0;
}
