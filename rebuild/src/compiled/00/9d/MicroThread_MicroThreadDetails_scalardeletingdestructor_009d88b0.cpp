// Retail 009D88B0: MicroThread::MicroThreadDetails scalar deleting destructor.
// Evidence: retail vtable 0x129B938 (RTTI COL 0x1355F5C, .?AVMicroThreadDetails@MicroThread@@,
// base .?AVMicroThreadInfo@MicroThread@@) slot 2 = 0x9D88B0; slot 0 = MarkAsDead (0x9D8630),
// slot 1 = MicroThreadStackSize (0x9D8890). Ego_r PDB order: MicroThreadStackSize then ??_EMicroThreadDetails.
// Out-of-line ~MicroThreadDetails is retail 0x9D88D0 (frees Stack storage, restores base vptr 0x129B928).
namespace MicroThread
{
    class MicroThreadInfo
    {
    public:
        virtual void MarkAsDead();
        virtual unsigned int MicroThreadStackSize() const = 0;
        virtual ~MicroThreadInfo() {}
        bool Dead;
    };

    class MicroThreadDetails : public MicroThreadInfo
    {
    public:
        MicroThreadDetails();
        virtual unsigned int MicroThreadStackSize() const;
        virtual ~MicroThreadDetails();
        unsigned char* StackFirst;
        unsigned char* StackLast;
        unsigned char* StackEnd;
    };

    MicroThreadDetails::MicroThreadDetails() : StackFirst(0), StackLast(0), StackEnd(0)
    {
        Dead = false;
    }
}
