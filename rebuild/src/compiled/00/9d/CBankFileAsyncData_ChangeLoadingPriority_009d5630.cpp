// Byte-exact reconstruction of CBankFileAsyncData::ChangeLoadingPriority @ 0x009d5630
typedef int EThreadedFilePriority;

struct Inner;

// callee at 0x98f0e0: thiscall on object at *(field_28 + 0x110),
// taking (Inner*, EThreadedFilePriority) as pushed args (callee-clean thiscall).
struct Consumer {
    char pad[0x100];
    void SetPriority(Inner* item, EThreadedFilePriority prio); // thiscall member, extern
};

struct Manager {
    char pad[0x110];
    Consumer* field_110; // +0x110
};

struct CBankFileAsyncData {
    char pad0[0x18];
    Inner* field_18;   // +0x18
    char pad1[0x28 - 0x1c];
    Manager* field_28; // +0x28
};

void __fastcall ChangeLoadingPriority(CBankFileAsyncData* self, void* /*edx*/, EThreadedFilePriority prio)
{
    Inner* item = self->field_18;
    if (item) {
        Consumer* c = self->field_28->field_110;
        c->SetPriority(item, prio);
    }
}