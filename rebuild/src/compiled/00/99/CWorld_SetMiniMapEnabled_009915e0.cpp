// Byte-exact reconstruction of CWorld::SetMiniMapEnabled @ 0x009915e0
struct CWorld {
    virtual void slot00();          // +0x00
    virtual void slot04();          // +0x04
    virtual void slot08();          // +0x08
    virtual void slot0c();          // +0x0c
    virtual void slot10();
    virtual void slot14();
    virtual void slot18();
    virtual void slot1c();
    virtual void slot20();
    virtual void slot24();
    virtual void slot28();
    virtual void slot2c();
    virtual void slot30();
    virtual void slot34();
    virtual void slot38();
    virtual void slot3c();
    virtual void slot40();
    virtual void slot44();
    virtual void OnMiniMapDisabled(int arg); // +0x48
    char pad[0x25 - 4];
    unsigned char miniMapEnabled; // +0x25

    void SetMiniMapEnabled(bool enabled);
};

void CWorld::SetMiniMapEnabled(bool enabled)
{
    if (this->miniMapEnabled == (unsigned char)enabled)
        return;
    this->miniMapEnabled = (unsigned char)enabled;
    if (enabled)
        return;
    this->OnMiniMapDisabled(0);
}