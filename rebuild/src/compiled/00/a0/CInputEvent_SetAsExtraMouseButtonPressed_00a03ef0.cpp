#pragma optimize("s",on)

struct C2DVector {
    float X;
    float Y;
};

enum EInputDeviceType {
    INPUT_DEVICE_MOUSE = 3
};

enum EInputEventType {
    INPUT_EVENT_EXTRA_MOUSE_PRESSED_BASE = 22
};

struct CInputEventRetail {
    unsigned char EarlierData[0x18];
    C2DVector MousePosition;
    EInputDeviceType DeviceType;
    long DeviceNumber;
    EInputEventType Type;
    float CurrentTime;
    float StartTime;

    void SetAsExtraMouseButtonPressed(
        const C2DVector& position,
        double time,
        long button);
};

void CInputEventRetail::SetAsExtraMouseButtonPressed(
    const C2DVector& position,
    double time,
    long button)
{
    DeviceType = INPUT_DEVICE_MOUSE;
    Type = static_cast<EInputEventType>(
        INPUT_EVENT_EXTRA_MOUSE_PRESSED_BASE + button);
    MousePosition = position;
    StartTime = static_cast<float>(time);
    CurrentTime = static_cast<float>(time);
}
