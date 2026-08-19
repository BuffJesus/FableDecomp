// _Dest_val body inlined into a vector deleting destructor: release the counted-pointer
// slot, clear both words, then optionally free the object itself.
// __fastcall this=ecx, flags=stack (ret 4).
struct Owner;
struct Info {
    long rc;
    void (__fastcall *release)(Owner* self);
    Owner* owner;
};
struct Slot {
    void* p;
    Info* info;
    void* VecDel(unsigned flags);
};
extern "C" void __cdecl Free1(void* p);   // 0x00bfe9bc
void* Slot::VecDel(unsigned flags) {
    if (this->info) {
        --this->info->rc;
        if (this->info->rc == 0) {
            this->info->release(this->info->owner);
            Free1(this->info);
        }
    }
    this->p = 0;
    this->info = 0;
    if (flags & 1) Free1(this);
    return this;
}
