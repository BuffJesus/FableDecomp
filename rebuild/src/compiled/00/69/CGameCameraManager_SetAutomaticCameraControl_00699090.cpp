// CGameCameraManager::SetAutomaticCameraControl @ 00699090  __fastcall void(this)

struct ScopedGuard {
    void* p;                 // 4-byte object
    ScopedGuard(const char* s, int n);
    ~ScopedGuard();
};

struct Inner {
    char x;
};

struct Sub100 {
    Inner* get(ScopedGuard* g);   // __fastcall, g on stack, returns Inner*
};

extern const char kStrA[];
extern const char kStrB[];

struct CGameCameraManager {
    char pad[0x100];
    Sub100 sub100;           // 0x100
    char pad1[0x2c - sizeof(Sub100)];
    char flag12c;            // 0x12c
    char flag12d;            // 0x12d
    char pad2[6];
    int  h134;               // 0x134
    void Extra();            // __fastcall
    void SetAutomaticCameraControl();
};

extern "C" void __fastcall Inner_call_edx(Inner* self, int edx);

void CGameCameraManager::SetAutomaticCameraControl()
{
    if (this->flag12c == 0) {
        this->flag12c = 0;
        ScopedGuard g(kStrA, -1);
        int h = this->h134;
        Inner* p = this->sub100.get(&g);
        Inner_call_edx(p, h);
        return;
    }
    {
        ScopedGuard g(kStrB, -1);
        int h = this->h134;
        Inner* p = this->sub100.get(&g);
        Inner_call_edx(p, h);
    }
    this->Extra();
    this->flag12d = 0;
    this->flag12c = 0;
}