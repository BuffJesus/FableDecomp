class CTCPhysicsStandard
{
public:
    unsigned char pad0[0x1c8];
    unsigned char bit0 : 1;
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
    unsigned char bit3 : 1;
    unsigned char bit4 : 1;
    unsigned char bit5 : 1;
    unsigned char bit6 : 1;
    unsigned char bit7 : 1;

    bool __fastcall CanCollideWithLandscape(void* /*edx*/);
};

bool __fastcall CTCPhysicsStandard::CanCollideWithLandscape(void* /*edx*/)
{
    return bit4;
}