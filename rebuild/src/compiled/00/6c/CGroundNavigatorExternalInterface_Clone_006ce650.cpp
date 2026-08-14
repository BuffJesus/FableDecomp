// CGroundNavigatorExternalInterface::Clone  0x006ce650
struct CNavigatorExternalInterface;
extern "C" void* __cdecl op_new(unsigned int sz);
struct CGroundNavigatorExternalInterface {
    CNavigatorExternalInterface* Construct(CGroundNavigatorExternalInterface* src);
    CNavigatorExternalInterface* Clone();
};
CNavigatorExternalInterface* CGroundNavigatorExternalInterface::Clone()
{
    void* p = op_new(0xc);
    if (p) {
        return ((CGroundNavigatorExternalInterface*)p)->Construct(this);
    }
    return 0;
}