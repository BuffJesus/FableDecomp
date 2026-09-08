struct C2DVector { float X; float Y; };
enum EInputDeviceType { INPUT_DEVICE_MOUSE = 3 };
enum EInputEventType { INPUT_EVENT_LEFT_MOUSE_PRESSED = 4 };

struct CInputEventRetail {
    unsigned char EarlierData[0x18];
    C2DVector MousePosition;
    EInputDeviceType DeviceType;
    long DeviceNumber;
    EInputEventType Type;
    float CurrentTime;
    float StartTime;
    void SetAsLeftMouseButtonPressed(const C2DVector& position, double time);
};

void CInputEventRetail::SetAsLeftMouseButtonPressed(
    const C2DVector& position, double time)
{
    DeviceType = INPUT_DEVICE_MOUSE;
    Type = INPUT_EVENT_LEFT_MOUSE_PRESSED;
    MousePosition = position;
    StartTime = static_cast<float>(time);
    CurrentTime = static_cast<float>(time);
}
