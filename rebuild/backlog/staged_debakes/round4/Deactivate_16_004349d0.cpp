// Guarded two-flag clear. __fastcall this=ecx, no args.
#pragma pack(push,1)
struct T {
    char pad00[0x70];
    char active;
    char pad71[0x0b];
    char shown;     // this+0x7c
    char pending;   // this+0x7d
    void Deactivate();
};
#pragma pack(pop)
void T::Deactivate() {
    if (this->active) {
        this->pending = 0;
        this->shown = 1;
    }
}
