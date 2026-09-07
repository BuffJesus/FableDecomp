class InputEventSubobject
{
public:
    unsigned long GetKey();
    void GetFramesRemaining(unsigned long key);
};
class InputEventOwner
{
public:
    unsigned char pad00[0x80];
    InputEventSubobject event80;
    void InputEvent_CheckKeyAndGetFrames(unsigned long key);
};
void InputEventOwner::InputEvent_CheckKeyAndGetFrames(unsigned long key)
{
    if (key < event80.GetKey())
        event80.GetFramesRemaining(key);
}