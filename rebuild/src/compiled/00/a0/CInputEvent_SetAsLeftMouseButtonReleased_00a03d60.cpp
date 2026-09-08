#pragma optimize("s",on)

struct C2DVector {
    float X;
    float Y;
};

enum EInputDeviceType {
    INPUT_DEVICE_MOUSE = 3
};

enum EInputEventType {
    INPUT_EVENT_LEFT_MOUSE_RELEASED = 6
};

struct CInputEventRetail {
    unsigned char EarlierData[0x18];
    C2DVector MousePosition;
    EInputDeviceType DeviceType;
    long DeviceNumber;
    EInputEventType Type;
    float CurrentTime;
    float StartTime;

    void SetAsLeftMouseButtonReleased(
        const C2DVector& position,
        double startTime,
        double currentTime);
};

void CInputEventRetail::SetAsLeftMouseButtonReleased(
    const C2DVector& position,
    double startTime,
    double currentTime)
{
    DeviceType = INPUT_DEVICE_MOUSE;
    Type = INPUT_EVENT_LEFT_MOUSE_RELEASED;
    MousePosition = position;
    StartTime = static_cast<float>(startTime);
    CurrentTime = static_cast<float>(currentTime);
}
