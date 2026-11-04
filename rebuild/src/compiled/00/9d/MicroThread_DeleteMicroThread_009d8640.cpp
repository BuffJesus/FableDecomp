// MicroThread::DeleteMicroThread (Ego_r ?DeleteMicroThread@MicroThread@@YIXPAVMicroThreadInfo@1@@Z).
// Retail 0x9D8640; callers ~CSuspendableProcessMicrothread (0xA4463E) and
// CSuspendableProcessMicrothread::Restart (0xA447E8) pass PMicrothread (+0x10).
// Retail vtable 0x129B938: [0] MarkAsDead 0x9D8630, [1] MicroThreadStackSize 0x9D8890, [2] vector deleting dtor 0x9D88B0.
namespace MicroThread
{
    class MicroThreadInfo
    {
    public:
        virtual void MarkAsDead();
        virtual unsigned int MicroThreadStackSize() const = 0;
    protected:
        virtual ~MicroThreadInfo();
        friend void __fastcall DeleteMicroThread(MicroThreadInfo* info);
    public:
        bool Dead;
    };

    void __fastcall DeleteMicroThread(MicroThreadInfo* info)
    {
        delete info;
    }
}
