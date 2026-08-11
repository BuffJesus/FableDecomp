extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void *pcs);

struct CCriticalSectionLock {
    void *pcs;               // +0
    unsigned char locked;    // +4
    ~CCriticalSectionLock();
};

CCriticalSectionLock::~CCriticalSectionLock()
{
    if (locked) {
        LeaveCriticalSection(pcs);
        locked = 0;
    }
}