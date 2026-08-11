// CPersistTraits<long>::TransferOut  @ 0x00c20400
// __fastcall void(this, arg)  -- retail cleans ret 4 (one stack arg used)

struct CPersistContext;
struct CCharString;

// sub-object living at this+8; its Write is a real out-of-line __fastcall helper
struct PersistWriter
{
    void Write(void* p);   // -> call 0xc1bb60
};

struct CPersistTraits_long
{
    unsigned int flags;    // +0
    int pad;               // +4
    PersistWriter writer;  // +8

    void TransferOut(void* arg);
};

void CPersistTraits_long::TransferOut(void* arg)
{
    writer.Write(arg);
    flags |= 0x100;
}