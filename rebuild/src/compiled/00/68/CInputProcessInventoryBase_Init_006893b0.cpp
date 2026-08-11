struct CInputProcessInventoryBase {
    char pad[0x28];
    int m28;
    int m2c;
    char m30;
    void Init();
};

void CInputProcessInventoryBase::Init()
{
    m30 = 0;
    m28 = 0;
    m2c = 0;
}