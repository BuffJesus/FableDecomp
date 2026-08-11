#include <cstdio>

struct CTCPhysicsNavigator {
    char pad0[8];
    bool enabled;
    void EnableNavigator(bool enable);
};

void CTCPhysicsNavigator::EnableNavigator(bool enable)
{
    this->enabled = enable;
}

int main()
{
    CTCPhysicsNavigator nav;
    nav.enabled = false;
    nav.EnableNavigator(true);
    if (nav.enabled != true) { printf("FAIL1\n"); return 1; }
    nav.EnableNavigator(false);
    if (nav.enabled != false) { printf("FAIL2\n"); return 1; }
    printf("NAV_ENABLE_OK\n");
    return 0;
}