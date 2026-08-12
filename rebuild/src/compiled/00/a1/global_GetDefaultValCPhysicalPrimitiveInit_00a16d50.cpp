// Sub-object living at offset +0x1c inside CPhysicalPrimitiveInit.
struct SubInit {
    void Init();  // out-of-line -> call 0x99e4b0
};

struct CPhysicalPrimitiveInit {
    int   a;        // +0x00
    int   b;        // +0x04
    char  pad[0x14];// +0x08 .. +0x1b
    SubInit sub;    // +0x1c
    CPhysicalPrimitiveInit* GetDefaultVal();
};

CPhysicalPrimitiveInit* CPhysicalPrimitiveInit::GetDefaultVal()
{
    a = 0;
    b = 0;
    sub.Init();
    return this;
}