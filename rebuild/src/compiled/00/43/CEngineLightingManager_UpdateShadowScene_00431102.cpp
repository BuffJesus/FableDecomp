#pragma optimize("s",on)
// Profiled two-case dispatch with a zero-initialised local handle.
// __fastcall this=ecx, arg=stack (ret 4).
struct Ctx { int value; };
struct Sink { void Take(int value); };
struct Handle {
    int value;
    void Fill(void* source, Ctx* arg);
};
#pragma pack(push,1)
struct T {
    char pad00[0x18];
    int mode;
    char pad1c[0x08];
    void* source;    // this+0x24
    Sink* sink;      // this+0x28
    void UpdateShadowScene(Ctx* arg);
};
#pragma pack(pop)
extern "C" void __stdcall BeginProfile(const char* tag);
extern const char g_profileTag[];
void T::UpdateShadowScene(Ctx* arg) {
    BeginProfile(g_profileTag);
    Handle local;
    local.value = 0;
    switch (this->mode) {
    case 2:
        local.Fill(this->source, arg);
        break;
    case 3:
        this->sink->Take(arg->value);
        break;
    }
}