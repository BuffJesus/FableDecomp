// CNewFrontendGameComponent::LoadGame @ 0x00594F36
//
// Retail prepares the component through vtable slot 0x10, marks the pending
// load at +0x29, then assigns the requested name into +0x84.  These typed
// members preserve that order and the original VC7.1 ABI without assembly.

#pragma optimize("s", on)

struct FrontEndWideString_00594f36
{
    void* data;
    FrontEndWideString_00594f36& operator=(
        const FrontEndWideString_00594f36& source);
};

class CNewFrontendGameComponent_00594f36
{
public:
    virtual void Slot00();
    virtual void Slot01();
    virtual void Slot02();
    virtual void Slot03();
    virtual void PrepareLoad();

    unsigned char padding_004[0x25];
    unsigned char load_pending;
    unsigned char padding_02a[0x5a];
    FrontEndWideString_00594f36 load_name;

    void LoadGame(const FrontEndWideString_00594f36& name);
};

void CNewFrontendGameComponent_00594f36::LoadGame(
    const FrontEndWideString_00594f36& name)
{
    PrepareLoad();
    load_pending = 1;
    load_name = name;
}

#pragma optimize("s", off)
