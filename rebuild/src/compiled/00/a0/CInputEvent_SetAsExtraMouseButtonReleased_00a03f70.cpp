#pragma optimize("s",on)

struct C2DVector {
    float X;
    float Y;
};

enum EInputDeviceType {
    INPUT_DEVICE_MOUSE = 3
};

enum EInputEventType {
    INPUT_EVENT_EXTRA_MOUSE_RELEASED_BASE = 32
};

// The retail input event has eight more leading bytes than the PDB donor.
struct CInputEventRetail {
    unsigned char EarlierData[0x18];
    C2DVector MousePosition;
    EInputDeviceType DeviceType;
    long DeviceNumber;
    EInputEventType Type;
    float CurrentTime;
    float StartTime;

    void SetAsExtraMouseButtonReleased(
        const C2DVector& position,
        double startTime,
        double currentTime,
        long extraButton);
};

void CInputEventRetail::SetAsExtraMouseButtonReleased(
    const C2DVector& position,
    double startTime,
    double currentTime,
    long extraButton)
{
    DeviceType = INPUT_DEVICE_MOUSE;
    Type = static_cast<EInputEventType>(
        INPUT_EVENT_EXTRA_MOUSE_RELEASED_BASE + extraButton);
    MousePosition = position;
    StartTime = static_cast<float>(startTime);
    CurrentTime = static_cast<float>(currentTime);
}
