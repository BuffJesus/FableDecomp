#include <stdio.h>

struct CMiniMapDisplay {
    char pad[0xbc];
    unsigned char dirtyFlag;
    int refreshCalled;
    void RefreshMarkers();
    void UpdateMarkers(bool immediate);
};

void CMiniMapDisplay::RefreshMarkers()
{
    this->refreshCalled = 1;
}

void CMiniMapDisplay::UpdateMarkers(bool immediate)
{
    if (immediate)
        RefreshMarkers();
    else
        this->dirtyFlag = 1;
}

int main()
{
    CMiniMapDisplay d;
    d.dirtyFlag = 0;
    d.refreshCalled = 0;
    d.UpdateMarkers(false);
    if (d.dirtyFlag != 1 || d.refreshCalled != 0) { printf("FAIL1\n"); return 1; }

    d.dirtyFlag = 0;
    d.refreshCalled = 0;
    d.UpdateMarkers(true);
    if (d.refreshCalled != 1 || d.dirtyFlag != 0) { printf("FAIL2\n"); return 1; }

    printf("UPDATEMARKERS_OK\n");
    return 0;
}