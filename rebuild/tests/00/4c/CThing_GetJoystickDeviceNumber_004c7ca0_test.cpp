#include <cstdio>

struct CInputProvider {
    long value;
    long GetDeviceNumber();
};

struct CInputManager {
    CInputProvider* table;
    CInputProvider* GetProvider(int deviceIndex);
};

struct CInputManagerHolder {
    char pad[0x30];
    CInputManager* mgr;
};

CInputManagerHolder* g_inputHolder;

struct CThing {
    char pad[0x90];
    signed char joystickDeviceNumber;
    long GetJoystickDeviceNumber();
};

static int g_lastIndex = -999;
static CInputManager* g_lastMgr = 0;
static CInputProvider g_prov;

CInputProvider* CInputManager::GetProvider(int deviceIndex) {
    g_lastMgr = this;
    g_lastIndex = deviceIndex;
    g_prov.value = deviceIndex + 1000;
    return &g_prov;
}
long CInputProvider::GetDeviceNumber() {
    return this->value * 2;
}

long CThing::GetJoystickDeviceNumber() {
    return g_inputHolder->mgr->GetProvider(this->joystickDeviceNumber)->GetDeviceNumber();
}

int main() {
    CInputManager mgr;
    CInputManagerHolder holder;
    holder.mgr = &mgr;
    g_inputHolder = &holder;

    CThing t;
    t.joystickDeviceNumber = (signed char)-3;

    long r = t.GetJoystickDeviceNumber();

    // -3+1000 = 997 ; 997*2 = 1994
    if (g_lastMgr == &mgr && g_lastIndex == -3 && r == 1994) {
        printf("JOYDEV_OK r=%ld idx=%d\n", r, g_lastIndex);
        return 0;
    }
    printf("FAIL r=%ld idx=%d\n", r, g_lastIndex);
    return 1;
}