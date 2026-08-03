class CThingImpl {
public:
    unsigned long pad_0x0;      // +0x0
    short pad_0x4;              // +0x4
    short currentMapNumber;     // +0x6

    void __fastcall SetCurrentMapNumber(void* /*edx_unused*/, short mapNumber);
};

void __fastcall CThingImpl::SetCurrentMapNumber(void* /*edx_unused*/, short mapNumber)
{
    currentMapNumber = mapNumber;
}